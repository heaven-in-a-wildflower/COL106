#include "Chaining.h"

// Helper Functions
void insertToSorted(std::vector<Account> &vec, Account &acc, int l, int u)
{
    // binary search based algorithm
    while (l <= u)
    {
        int mid = (l + u) / 2;
        if ((vec[mid].balance) == acc.balance)
        {
            vec.insert(vec.begin() + mid, acc);
            return;
        }
        else if ((vec[mid].balance) < acc.balance)
        {
            l = mid + 1;
        }
        else if ((vec[mid].balance) > acc.balance)
        {
            u = mid - 1;
        }
    }
    vec.insert(vec.begin() + l, acc);
}

void swap(std::vector<int> &vec, int i, int j)
{
    int temp = vec[i];
    vec[i] = vec[j];
    vec[j] = temp;
}

int partition(std::vector<int> &vec, int start, int end)
{
    int pivot = vec[end];
    int pIndex = start;
    for (int i = start; i < end; i++)
    {
        if (vec[i] <= pivot)
        {
            swap(vec, i, pIndex);
            pIndex++;
        }
    }
    swap(vec, pIndex, end);
    return pIndex;
}

void qsort(std::vector<int> &vec, int start, int end)
{
    if (start < end)
    {
        int pIndex = partition(vec, start, end);
        qsort(vec, start, pIndex - 1);
        qsort(vec, pIndex + 1, end);
    }
}

// Required methods
Chaining::Chaining()
{
    bankStorage2d.resize(bucket_array_size);
}

double Chaining::loadFactor()
{
    return double(table_size)/(bucket_array_size);
}
void Chaining::createAccount(std::string id, int count)
{
    int index = hash(id);
    Account acc;
    acc.id = id;
    acc.balance = count;
    insertToSorted(bankStorage2d[index], acc, 0, bankStorage2d[index].size() - 1);
    table_size++;
    load_factor = double(table_size)/(bucket_array_size);
    if (load_factor > 0.7)
        resize(true);
}

std::vector<int> Chaining::getTopK(int k)
{
    std::vector<int> top_balances;
    for (int i = 0; i < bucket_array_size; i++)
    {
        if (!bankStorage2d[i].empty())
        {
            top_balances.push_back(bankStorage2d[i].back().balance);
        }
    }
    qsort(top_balances, 0, top_balances.size() - 1);
    std::vector<int> top_k_balances;
    for (int i = 0; i < k; i++)
    {
        top_k_balances.push_back(top_balances.back());
        top_balances.pop_back();
    }
    return top_k_balances;
}

int Chaining::getBalance(std::string id)
{
    int index = hash(id);
    for (int i = 0; i < bankStorage2d[index].size(); i++)
    {
        if (bankStorage2d[index][i].id == id)
        {
            return bankStorage2d[index][i].balance;
        }
    }
    return -1;
}

void Chaining::addTransaction(std::string id, int count)
{
    int index = hash(id);
    for (int i = 0; i < bankStorage2d[index].size(); i++)
    {
        if (bankStorage2d[index][i].id == id)
        {
            bankStorage2d[index][i].balance += count;
            Account acc = bankStorage2d[index][i];
            bankStorage2d[index].erase(bankStorage2d[index].begin() + i);
            insertToSorted(bankStorage2d[index], acc, 0, bankStorage2d[index].size() - 1);
            return;
        }
    }
    Account acc;
    acc.id = id;
    acc.balance = count;
    insertToSorted(bankStorage2d[index], acc, 0, bankStorage2d[index].size() - 1);
    table_size++;
    load_factor = double(table_size)/(bucket_array_size);
}

bool Chaining::doesExist(std::string id)
{
    int index = hash(id);
    for (int i = 0; i < bankStorage2d[index].size(); i++)
    {
        if (bankStorage2d[index][i].id == id)
        {
            return true;
        }
    }
    return false;
}

bool Chaining::deleteAccount(std::string id)
{
    int index = hash(id);
    for (int i = 0; i < bankStorage2d[index].size(); i++)
    {
        if (bankStorage2d[index][i].id == id)
        {
            bankStorage2d[index].erase(bankStorage2d[index].begin() + i);
            table_size--;
            load_factor = double(table_size)/(bucket_array_size);
            if (load_factor < 0.34 && ptr > 0)
                resize(false);
            load_factor = double(table_size)/(bucket_array_size);
            return true;
        }
    }
    return false;
}

int Chaining::databaseSize()
{
    return table_size;
}

int Chaining::hash(std::string id)
{
    // Rolling hash
    const unsigned int p = 31;
    int hash_value = 0;
    for (char c : id)
    {
        if (std::isalpha(c))
        {
            hash_value = (hash_value * p + (c - 'A' + 1)) % bucket_array_size;
        }
        else if (std::isdigit(c))
        {
            hash_value = (hash_value * p + (c - '0' + 27)) % bucket_array_size;
        }
    }
    return hash_value;
}

void Chaining::resize(bool flag)
{
    if (flag == true)
        ptr++;
    else
        ptr--;

    ptr = std::max(0, std::min(ptr, int((prime_list.size()) - 1)));

    std::vector<std::vector<Account>> new_bucket_array;
    
    bucket_array_size = prime_list[ptr]; //Very important
    
    new_bucket_array.resize(prime_list[ptr]);
    for (int i = 0; i < bankStorage2d.size(); i++)
    {
        for (int j = 0; j < bankStorage2d[i].size(); j++)
        {
            int idx = hash(bankStorage2d[i][j].id);
            insertToSorted(new_bucket_array[idx], bankStorage2d[i][j], 0, new_bucket_array[idx].size() - 1);
        }
    }
    bankStorage2d = new_bucket_array;
}
