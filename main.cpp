#include <bits/stdc++.h>
using namespace std;

class TrieNode
{
public:
    set<string> documents;
    TrieNode *children[52];
};

class TrieTree
{
public:
    TrieNode *root;

    void insert(string word, string doc)
    {
        auto child = [](char c)
        {
            return (c > 96) ? 2 * (c - 'a') + 1 : 2 * (c - 'A');
        };

        TrieNode *cur = root;

        for (int i = 0; i < word.size(); i++)
        {
            cur->children[child(word[i])] = (cur->children[child(word[i])] == nullptr) ? new TrieNode() : cur->children[child(word[i])];
            cur = cur->children[child(word[i])];

            if (i == word.size() - 1)
                cur->documents.insert(doc);
        }
    }

    TrieNode *find(string word)
    {
        auto child = [](char c)
        {
            return (c > 96) ? 2 * (c - 'a') + 1 : 2 * (c - 'A');
        };

        TrieNode *cur = root;

        for (int i = 0; i < word.size(); i++)
        {
            if (cur->children[child(word[i])] == nullptr)
                return nullptr;

            cur = cur->children[child(word[i])];

            if (i == word.size() - 1)
            {
                return cur;
            }
        }

        return nullptr;
    }

    bool search(string word, string doc)
    {
        TrieNode *aim = find(word);
        return (aim != nullptr) && (aim->documents.find(doc) != aim->documents.end());
    }

    bool remove(string word, string doc)
    {
        TrieNode *cur = find(word);
        if (!search(word, doc) || cur == nullptr)
            return false;
        cur->documents.erase(doc);
    }

    set<string> suggestionWord(string word, string doc)
    {
        auto child = [](int i)
        {
            return (i > 25) ? char('a' + i - 26) : char('A' + i);
        };

        set<string> nearWords;

        for (int j = 0; j < word.size(); j++)
        {
            for (int i = 0; i < 52; i++)
            {
                string temp = word;
                temp[j] = child(i);
                if (search(temp, doc))
                    nearWords.insert(temp);
            }
        }

        for (int j = 0; j < word.size(); j++)
        {
            for (int i = 0; i < 52; i++)
            {
                string temp = word;
                temp.insert(temp.begin() + j, child(i));
                if (search(temp, doc))
                    nearWords.insert(temp);
            }
        }

        for (int j = 0; j < word.size(); j++)
        {
            string temp = word;
            temp.erase(temp.begin() + j);
            if (search(temp, doc))
                nearWords.insert(temp);
        }

        return nearWords;
    }

    set<string> findPrefixes(TrieNode *aim, string doc, string word = "", set<string> *nearWord = new set<string>())
    {
        auto next = [](int i)
        {
            return (i % 2 == 0) ? i / 2 + 'A' : i / 2 + 'a';
        };

        if (aim->documents.find(doc) != aim->documents.end())
        {
            nearWord->insert(word);
        }

        bool flag = true;
        for (int i = 0; i < 52; i++)
        {
            if (aim->children[i] != nullptr)
            {
                word.push_back(next(i));
                findPrefixes(aim->children[i], doc, word, nearWord);
                word.pop_back();
            }
        }

        return *nearWord;
    }

    void printDictionary(TrieNode *aim, string doc, string word = "")
    {
        auto next = [](int i)
        {
            return (i % 2 == 0) ? i / 2 + 'A' : i / 2 + 'a';
        };

        if (aim->documents.find(doc) != aim->documents.end())
        {
            cout << word << "\n";
        }

        for (int i = 0; i < 52; i++)
        {
            if (aim->children[i])
            {
                word.push_back(next(i));
                printDictionary(aim->children[i], doc, word);
                word.pop_back();
            }
        }
    }

};

int main()
{
    TrieTree *t = new TrieTree();

    t->root = new TrieNode();

    t->insert("Matin", "A");//
    t->insert("mati", "A");//
    t->insert("main", "A");//
    t->insert("mtin", "A");//
    t->insert("atin", "A");//
    t->insert("matn", "A");//
    t->insert("Natin", "A");//
    t->insert("matn", "A");//
    t->insert("matnd", "A");
    t->insert("Matna", "A");
    t->insert("Matnaaa", "A");
    t->insert("Matyyna", "A");
    t->insert("Matinkkk", "A");//
    t->insert("Matinjppojpo", "A");//
    t->insert("Matinhjhjhkk", "A");//
    t->insert("Matinlp", "A");//


    set<string> words = t->suggestionWord("matin", "A");
    cout << "--------------------------------------\n";
    for (string s : words)
    {
        cout << s << "\n";
    }

    words = t->findPrefixes(t->find("Matin"), "A", "Matin");
    cout << "--------------------------------------\n";
    for (string s : words)
    {
        cout << s << "\n";
    }
    return 0;
}