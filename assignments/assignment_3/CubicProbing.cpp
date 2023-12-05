#include "CubicProbing.h"

// Helper functions
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

//Required methods

CubicProbing::CubicProbing()
{
    Account acc;
    acc.id = "free";
    acc.balance = 0;
    bankStorage1d.resize(array_size, acc);
}
void CubicProbing::createAccount(std::string id, int count) {
    Account acc;
    acc.id = id;
    acc.balance = count;
    for (int i = 0; i < array_size; i++)
    {
        int idx = (hash(id) + i*i*i) % array_size;
        if (bankStorage1d[idx].id == "free" || bankStorage1d[idx].id == "deleted")
        {
            bankStorage1d[idx] = acc;
            table_size++;
            load_factor = double(table_size) / array_size;
            return;
        }
    }
}

std::vector<int> CubicProbing::getTopK(int k) {
    std::vector<int> balances;
    for (int i = 0; i < array_size; i++)
    {
        if (bankStorage1d[i].id != "free" && bankStorage1d[i].id != "deleted")
        {
            balances.push_back(bankStorage1d[i].balance);
        }
    }
    qsort(balances, 0, balances.size() - 1);
    std::vector<int> top_k_balances;
    if(balances.size()>k)
    {
    for (int i = 0; i < k; i++)
    {
        top_k_balances.push_back(balances.back());
        balances.pop_back();
    }
    return top_k_balances;
    }
    else 
    {
        int i = 0;
        int j = balances.size()-1;
        while(i<j)
        {
            swap(balances,i,j);
            i++;
            j--;
        }
        return balances;
    }
}

int CubicProbing::getBalance(std::string id) {
   for (int i = 0; i < array_size; i++)
    {
        int idx = (hash(id) + i*i*i) % array_size;
        if (bankStorage1d[idx].id == id)
        {
            return bankStorage1d[idx].balance;
        }
        else if (bankStorage1d[idx].id == "free")
        {
            return -1;
        }
        else continue;
    }
    return -1;
}

void CubicProbing::addTransaction(std::string id, int count) {
    for (int i = 0; i < array_size; i++)
    {
        int idx = (hash(id) + i*i*i) % array_size;
        if (bankStorage1d[idx].id == id)
        {
            bankStorage1d[idx].balance += count;
            return;
        }
        else if (bankStorage1d[idx].id == "free")
        {
            Account acc;
            acc.id = id;
            acc.balance = count;
            bankStorage1d[idx] = acc;
            table_size++;
            load_factor = double(table_size) / array_size;
            return;
        }
    }
}

bool CubicProbing::doesExist(std::string id) {
    for (int i = 0; i < array_size; i++)
    {
        int idx = (hash(id) + i*i*i) % array_size;
        if (bankStorage1d[idx].id == id)
        {
            return true;
        }
        else if (bankStorage1d[idx].id == "free")
        {
            return false;
        }
    }
    return false;
}

bool CubicProbing::deleteAccount(std::string id) {
   for (int i = 0; i < array_size; i++)
    {
        int idx = (hash(id) + i*i*i) % array_size;
        if (bankStorage1d[idx].id == id)
        {
            Account acc;
            acc.id = "deleted";
            acc.balance = 0;
            bankStorage1d[idx] = acc;
            table_size--;
            load_factor = double(table_size) / array_size;
            return true;
        }
        else if (bankStorage1d[idx].id == "free")
        {
            return false;
        }
    }
    return false;
}
int CubicProbing::databaseSize() {
    return table_size;
}

int CubicProbing::hash(std::string id) {
 // Rolling hash
    const unsigned int p = 31;
    int hash_value = 0;
    for (char c : id)
    {
        if (std::isalpha(c))
        {
            hash_value = (hash_value * p + (c - 'A' + 1)) % array_size;
        }
        else if (std::isdigit(c))
        {
            hash_value = (hash_value * p + (c - '0' + 27)) % array_size;
        }
    }
    return hash_value;
}

