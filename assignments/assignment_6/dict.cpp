// Do NOT add any other includes
#include "dict.h"

uint32_t xxhash32(const void* input, size_t length)
{
    const uint32_t prime32_1 = 0x9e3779b1U;
    const uint32_t prime32_2 = 0x85ebca77U;
    const uint32_t prime32_3 = 0xc2b2ae3dU;
    const uint32_t prime32_4 = 0x27d4eb2fU;
    const uint32_t prime32_5 = 0x165667b1U;

    const uint8_t* p = static_cast<const uint8_t*>(input);
    const uint8_t* end = p + length;
    uint32_t h32;

    if (length >= 16) {
        const uint8_t* limit = end - 16;
        uint32_t v1 = 0;
        uint32_t v2 = 0;
        uint32_t v3 = 0;
        uint32_t v4 = 0;

        do {
            v1 = (v1 + static_cast<uint32_t>(p[0]) + (static_cast<uint32_t>(p[1]) << 8) + (static_cast<uint32_t>(p[2]) << 16) + (static_cast<uint32_t>(p[3]) << 24)) * prime32_2;
            v1 = (v1 << 13) | (v1 >> 19);
            v1 *= prime32_1;
            p += 4;

            v2 = (v2 + static_cast<uint32_t>(p[0]) + (static_cast<uint32_t>(p[1]) << 8) + (static_cast<uint32_t>(p[2]) << 16) + (static_cast<uint32_t>(p[3]) << 24)) * prime32_2;
            v2 = (v2 << 13) | (v2 >> 19);
            v2 *= prime32_1;
            p += 4;

            v3 = (v3 + static_cast<uint32_t>(p[0]) + (static_cast<uint32_t>(p[1]) << 8) + (static_cast<uint32_t>(p[2]) << 16) + (static_cast<uint32_t>(p[3]) << 24)) * prime32_2;
            v3 = (v3 << 13) | (v3 >> 19);
            v3 *= prime32_1;
            p += 4;

            v4 = (v4 + static_cast<uint32_t>(p[0]) + (static_cast<uint32_t>(p[1]) << 8) + (static_cast<uint32_t>(p[2]) << 16) + (static_cast<uint32_t>(p[3]) << 24)) * prime32_2;
            v4 = (v4 << 13) | (v4 >> 19);
            v4 *= prime32_1;
            p += 4;
        } while (p <= limit);

        h32 = (v1 << 1) | (v1 >> 31);
        h32 += (v2 << 7) | (v2 >> 25);
        h32 += (v3 << 12) | (v3 >> 20);
        h32 += (v4 << 18) | (v4 >> 14);
    } else {
        h32 = length + prime32_5;
    }

    while (p + 4 <= end) {
        h32 += (static_cast<uint32_t>(p[0]) + (static_cast<uint32_t>(p[1]) << 8) + (static_cast<uint32_t>(p[2]) << 16) + (static_cast<uint32_t>(p[3]) << 24)) * prime32_3;
        h32 = (h32 << 17) | (h32 >> 15);
        h32 *= prime32_4;
        p += 4;
    }

    while (p < end) {
        h32 += static_cast<uint32_t>(*p) * prime32_5;
        h32 = (h32 << 11) | (h32 >> 21);
        h32 *= prime32_1;
        ++p;
    }

    h32 ^= h32 >> 15;
    h32 *= prime32_2;
    h32 ^= h32 >> 13;
    h32 *= prime32_3;
    h32 ^= h32 >> 16;

    return h32;
}

Dict::Dict() : table(2048), tableSize(2048), itemCount(0), resizeCount(0)
{
}

Dict::~Dict() {}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    const string delimiters = " .,-:!\"\'()?[];@";
    vector<string> tokens;
    size_t pos = 0;
    while ((pos = sentence.find_first_of(delimiters, 0)) != string::npos)
    {
        if (pos > 0)
            tokens.push_back(sentence.substr(0, pos));
        tokens.push_back(string(1, sentence[pos]));
        sentence.erase(0, pos + 1);
    }
    if (!sentence.empty())
        tokens.push_back(sentence);

    for (const string &token : tokens)
    {
        string sanitizedWord;
        for (char c : token)
        {
            if (c != '.' && c != ',' && c != '-' && c != ':' &&c != '!'  && int(c) != 39 && int(c) != 34 &&c != '(' &&c != ')' &&c != '?' &&c != '[' &&c != ']' &&c != ';' &&c != '@' &&c != ' ')
            {
                sanitizedWord += c;
            }
        }

        if (!sanitizedWord.empty())
        {
            this->addWord(sanitizedWord);
        }
    }
}

void Dict::addWord(const string &word)
{
    string lowercaseWord = word;
    for (char &c : lowercaseWord)
    {
        c = tolower(c);
    }

    uint32_t hash = xxhash32(lowercaseWord.c_str(), lowercaseWord.length());
    int index = hash % tableSize;

    // Check if the word is already in the list for this index
    for (WordCount &item : table[index])
    {
        if (item.word == lowercaseWord)
        {
            item.count++;
            return;
        }
    }

    // Word not found, add it to the list for this index
    WordCount newWordCount;
    newWordCount.word = lowercaseWord;
    newWordCount.count = 1;
    table[index].push_back(newWordCount);
    itemCount++;

    // Check if the table needs to be resized
    if (static_cast<double>(itemCount) / tableSize > 0.9)
    {
        resizeTable();
    }
}

int Dict::get_word_count(string word)
{
    string lowercaseWord = word;
    for (char &c : lowercaseWord)
    {
        c = tolower(c);
    }

    uint32_t hash = xxhash32(lowercaseWord.c_str(), lowercaseWord.length());
    int index = hash % tableSize;

    for (const WordCount &item : table[index])
    {
        if (item.word == lowercaseWord)
        {
            return item.count;
        }
    }

    return 0; // Word not found
}

void Dict::dump_dictionary(string filename)
{

    ofstream outputFile(filename);
    if (!outputFile)
    {
        cerr << "Error: Unable to open the output file." << endl;
        return;
    }

    for (const vector<Dict::WordCount> &chain : table)
    {
        for (const Dict::WordCount &item : chain)
        {
            outputFile << item.word << ", " << item.count << endl;
        }
    }
    outputFile.close();
}

void Dict::resizeTable()
{
    tableSize *= 2;
    vector<vector<WordCount>> newTable(tableSize);

    for (const vector<WordCount> &chain : table)
    {
        for (const WordCount &item : chain)
        {
            uint32_t hash = xxhash32(item.word.c_str(), item.word.length());
            int index = hash % tableSize;
            newTable[index].push_back(item);
        }
    }

    table = move(newTable);
    resizeCount++;
}
