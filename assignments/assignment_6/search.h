// Do NOT add any other includes
#include <string>
#include <vector>
#include <iostream>
#include "Node.h"

using namespace std;

class Info
{
public:
    int bc;
    int pg;
    int para;
    int sn;
    string sentence;
    int offset;
    Info(){}
    Info(int bc, int pg, int para, int sn, string sentence) : bc(bc), pg(pg), para(para), sn(sn), sentence(sentence) {}
};

class SearchEngine
{
private:
    vector<Info> storage;

public:
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node *search(string pattern, int &n_matches);
};