#include <bits/stdc++.h>
using namespace std;

class TrieNode
{
public:
    bool isEnd;
    TrieNode *children[52];
    TrieNode()
    {
        isEnd = false;
    }
};

class TrieTree
{
public:
    TrieNode *root;

    void insert(string word)
    {

        auto child = [](char c)
        {
            return (c > 96) ? 2*(c-'a')+1 : 2*(c-'A');
        };

        TrieNode *cur = root;

        for (int i = 0; i < word.size(); i++)
        {
            // int child = (word[i] > 96) ? word[i] - 'a' + 26 : word[i] - 'A';

            cur->children[child(word[i])] = (cur->children[child(word[i])] == nullptr) ? new TrieNode() : cur->children[child(word[i])];
            cur = cur->children[child(word[i])];

            cur->isEnd = (i == word.size() - 1) ? true : cur->isEnd;
        }
    }

    TrieNode *find(string word)
    {
        TrieNode *cur = root;

        for (int i = 0; i < word.size(); i++)
        {
            int child = (word[i] > 96) ? word[i] - 'a' + 26 : word[i] - 'A';
            if (cur->children[child] == nullptr)
                return nullptr;

            cur = cur->children[child];

            if (i == word.size() - 1)
            {
                if (cur->isEnd)
                    return cur;
                else
                    return nullptr;
            }
        }

        return nullptr;
    }

    bool search(string word)
    {
        return find(word) != nullptr;
    }

    bool remove(string word)
    {
        TrieNode *cur = find(word);
        if (cur == nullptr)
            return false;
        cur->isEnd = false;
    }

    vector<TrieNode *> *findNearest(TrieNode *aim, int hAim, int h = 0, vector<TrieNode *> *near = new vector<TrieNode *>())
    {
        if (h == hAim)
            return near;

        if (aim->isEnd)
            near->push_back(aim);

        for (int i = 0; i < 52; i++)
        {
            if (aim->children[i] != nullptr)
                return findNearest(aim->children[i], hAim, h + 1, near);
        }
    }

    void printDictionary(TrieNode *aim, string word = "")
    {
        if (aim->isEnd)
            cout << word << "\n";

        for (int i = 0; i < 52; i++)
        {
            if (aim->children[i])
            {
                char c = (i > 25) ? i + 'a' - 26 : i + 'A';

                word.push_back(c);
                printDictionary(aim->children[i], word);
                word.pop_back();
            }
        }
    }
};

int main()
{

    TrieTree *t = new TrieTree();

    t->root = new TrieNode();
    t->insert("hi");
    t->insert("himamad");
    t->insert("Matiiiiin");
    t->insert("aliii");

    t->printDictionary(t->root);

    return 0;
}
