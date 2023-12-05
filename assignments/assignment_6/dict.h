// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class Dict {
private:
    private:
    struct WordCount
    {
        string word;
        int count;
    };

    vector<vector<WordCount>> table;
    int tableSize;
    int itemCount;
    int resizeCount;

    void resizeTable();
    void addWord(const string &word);

public: 
    ofstream output;
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    int getResizeCount();

    double getAverageChainLength();
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};