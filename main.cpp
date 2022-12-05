#include <bits/stdc++.h>
using namespace std;

class TrieTree
{
public:
    class TrieNode
    {
    public:
        set<string> documents;
        TrieNode *children[52];
    };

    TrieNode *root;

    TrieTree()
    {
        root = new TrieNode();
    }

    void insertTrie(string word, string doc)
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

    bool searchTrie(string word, string doc)
    {
        TrieNode *aim = find(word);
        return (aim != nullptr) && (aim->documents.find(doc) != aim->documents.end());
    }

    bool removeTrie(string word, string doc)
    {
        TrieNode *cur = find(word);
        if (!searchTrie(word, doc) || cur == nullptr)
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
                if (searchTrie(temp, doc))
                    nearWords.insert(temp);
            }
        }

        for (int j = 0; j < word.size(); j++)
        {
            for (int i = 0; i < 52; i++)
            {
                string temp = word;
                temp.insert(temp.begin() + j, child(i));
                if (searchTrie(temp, doc))
                    nearWords.insert(temp);
            }
        }

        for (int j = 0; j < word.size(); j++)
        {
            string temp = word;
            temp.erase(temp.begin() + j);
            if (searchTrie(temp, doc))
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

    void printDictionary(TrieNode *aim, string doc, string word = "", int num = 0)
    {
        auto next = [](int i)
        {
            return (i % 2 == 0) ? i / 2 + 'A' : i / 2 + 'a';
        };

        if (aim->documents.find(doc) != aim->documents.end())
        {
            if (num % 10 == 0)
                cout << "\n";
            cout << word << " ";

            num++;
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

    set<string> *findDocuments(string word)
    {
        TrieNode *aim = find(word);
        return (aim) ? (&aim->documents) : new set<string>();
    }

    set<string> conditionExp(string exp)
    {
        vector<string> unionWord;
        vector<string> intersectionWord;
        vector<string> differenceWord;

        vector<string> allWords = treeSplit(exp);
        for (string word : allWords)
        {
            if (word[0] == '+')
            {
                word.erase(word.begin());
                unionWord.push_back(word);
            }
            else if (word[0] == '-')
            {
                word.erase(word.begin());
                differenceWord.push_back(word);
            }
            else
                intersectionWord.push_back(word);
        }

        set<string> unWord;
        set<string> inWord;
        set<string> dfWord;

        inWord = *findDocuments(intersectionWord[0]);
        for (string word : intersectionWord)
        {
            set<string> temp = *findDocuments(word);
            set<string> t = inWord;
            for (string w : t)
            {
                if (temp.find(w) == temp.end())
                {
                    inWord.erase(w);
                }
            }
        }

        for (string word : unionWord)
        {
            set<string> temp = *findDocuments(word);
            unWord.insert(temp.begin(), temp.end());
        }

        for (string word : differenceWord)
        {
            set<string> temp = *findDocuments(word);
            dfWord.insert(temp.begin(), temp.end());
        }

        set<string> aim;
        aim = inWord;
        set<string> t = aim;
        for (string word : t)
        {
            if (!unWord.empty() && unWord.find(word) == unWord.end())
            {
                aim.erase(word);
            }
        }

        t = aim;
        for (string word : t)
        {
            if (dfWord.find(word) != dfWord.end())
            {
                aim.erase(word);
            }
        }

        return aim;
    }

    vector<string> treeSplit(string s)
    {
        s.push_back(' ');
        vector<string> aim;

        string delimiter = " ";

        size_t pos = 0;
        string token;
        while ((pos = s.find(delimiter)) != string::npos)
        {
            token = s.substr(0, pos);
            aim.push_back(token);
            s.erase(0, pos + delimiter.length());
        }

        return aim;
    }
};

string edit(string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')))
        {
            s[i] = ' ';
        }
    }
    return s;
}

vector<string> split(string s)
{
    s.push_back(' ');
    vector<string> aim;

    string delimiter = " ";

    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos)
    {
        token = s.substr(0, pos);
        aim.push_back(token);
        s.erase(0, pos + delimiter.length());
    }

    return aim;
}

void readFile(TrieTree *tree)
{
    string path = "/media/influx/Programming/Projects/project-4-InFluX-M/EnglishData/";
    for (int i = 1; i <= 1000; i++)
    {
        string pathN = path;
        pathN += to_string(i);
        pathN = pathN + ".txt";
        ifstream in(pathN);

        while (!in.eof())
        {
            string text;
            getline(in, text);
            text = edit(text);

            for (string word : split(text))
            {
                if (word.size() != 1 && word != " " && !word.empty())
                {
                    tree->insertTrie(word, to_string(i));
                }
            }
        }
    }
}

void panel(TrieTree *tree)
{
    cout << "1- Insert Word\n"
         << "2- Remove Word\n"
         << "3- Search Word\n"
         << "4- Suggestion Words\n"
         << "5- Prefixes Words\n"
         << "6- Print (Dictionary)\n"
         << "7- Conditional Expression\n"
         << "8- Read Files \n\n";

    cout << "Command: ";
    int expression;
    cin >> expression;

    string word;
    string doc;
    set<string> suggestions;
    set<string> prefixes;
    string exp;
    set<string> documents;

    switch (expression)
    {
    case 1:
        cout << "Document Name: ";
        cin >> doc;
        cout << "Word: ";
        cin >> word;

        tree->insertTrie(word, doc);

        cout << "DONE :)\n";

        break;

    case 2:
        cout << "Document Name: ";
        cin >> doc;
        cout << "Word: ";
        cin >> word;

        if (tree->removeTrie(word, doc))
            cout << "DONE :)\n";
        else
            cout << "FAIL :(\n";

        break;

    case 3:
        cout << "Document Name: ";
        cin >> doc;
        cout << "Word: ";
        cin >> word;

        if (tree->searchTrie(word, doc))
            cout << "FIND :)\n";
        else
            cout << "NOT FIND :(\n";

        break;

    case 4:
        cout << "Document Name: ";
        cin >> doc;
        cout << "Word: ";
        cin >> word;

        suggestions = tree->suggestionWord(word, doc);
        if (suggestions.empty())
            cout << "The Suggestion Words was not Found :(\n";
        else
        {
            int i = 0;
            for (string w : suggestions)
            {
                if (i % 10 == 0)
                    cout << "\n";
                cout << w << " ";

                i++;
            }
        }
        break;

    case 5:
        cout << "Document Name: ";
        cin >> doc;
        cout << "Word: ";
        cin >> word;

        prefixes = tree->findPrefixes(tree->root, doc, word);
        if (prefixes.empty())
            cout << "The Prefixes was not Found :(\n";
        else
        {
            int i = 0;
            for (string w : prefixes)
            {
                if (i % 10 == 0)
                    cout << "\n";
                cout << w << " ";

                i++;
            }
        }
        break;

    case 6:
        cout << "Document Name: ";
        cin >> doc;

        tree->printDictionary(tree->root, doc);
        break;

    case 7:
        cin.ignore();
        getline(cin, exp);
        documents = tree->conditionExp(exp);
        if (documents.empty())
            cout << "The Documents was not Found :(\n";
        else
        {
            int i = 0;
            for (string w : documents)
            {
                if (i % 10 == 0)
                    cout << "\n";
                cout << w << ".txt" << " ";

                i++;
            }
        }
        break;

    case 8:
        readFile(tree);
        cout << "DONE :)";
        break;

    default:
        break;
    }

    cout << "\n\n";
    panel(tree);
}

void mainPanel()
{
    TrieTree tree;
    panel(&tree);
}

int main()
{
    mainPanel();
    return 0;
}