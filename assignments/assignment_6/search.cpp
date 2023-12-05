#include "search.h"

const int chars = 256;

void CharCheck(string str, int size, int badchar[chars])
{
    int i;
    for (i = 0; i < chars; i++)
    {
        badchar[i] = -1;
    }
    for (i = 0; i < size; i++)
    {
        badchar[(int)str[i]] = i;
    }
}

vector<int> BMsearch(int m, string txt, string pat, int badchar[chars])
{
    int n = txt.length();
    vector<int> results;
    int s = 0;
    while (s <= (n - m))
    {
        int j = m - 1;
        while (j >= 0)
        {
            if (pat[j] != txt[s + j])
            {
                break;
            }
            j--;
        }
        if (j >= 0)
        {
            s += max(1, j - badchar[txt[s + j]]);
        }
        else
        {
            results.push_back(s);
            if (s < n - m)
            {
                s = s + m - badchar[txt[s + m]];
            }
            else
            {
                s = s + 1;
            }
        }
    }
    return results;
}

SearchEngine::SearchEngine()
{
}

SearchEngine::~SearchEngine()
{
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    string lower = "";
    for (auto d : sentence)
    {
        lower += tolower(d);
    }
    storage.push_back(Info(book_code, page, paragraph, sentence_no, lower));
    return;
}

Node *SearchEngine::search(string pattern, int &n_matches)
{
    n_matches = 0;
    string lower = "";
    for (auto d : pattern)
    {
        lower += tolower(d);
    }

    vector<Node *> matches;
    int m = lower.size();
    int badchar[chars];
    matches.clear();
    CharCheck(lower, m, badchar);
    for (auto ser : storage)
    {
        vector<int> result = BMsearch(m, ser.sentence, lower, badchar);
        for (auto i : result)
        {
            Node *inf = new Node(ser.bc, ser.pg, ser.para, ser.sn, i);
            matches.push_back(inf);
            n_matches++;
        }
    }
    n_matches = matches.size();
    if (matches.size() > 1)
    {
        for (int i = 1; i < n_matches - 1; i++)
        {
            matches[i]->left = matches[i - 1];
            matches[i]->right = matches[i + 1];
        }
        matches.front()->right = matches[1];
        matches.back()->left = matches[n_matches - 2];
        matches.front()->left = NULL;
        matches.back()->right = NULL;
        return matches.front();
    }
    else if (n_matches == 1)
    {
        return matches[0];
    }
    return nullptr;
}
