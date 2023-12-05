#include "LinearProbing.h"

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

// Required methods
LinearProbing::LinearProbing()
{
    Account acc;
    acc.id = "free";
    acc.balance = 0;
    bankStorage1d.resize(array_size, acc);
}

void LinearProbing::createAccount(std::string id, int count)
{
    Account acc;
    acc.id = id;
    acc.balance = count;
    for (int i = 0; i < array_size; i++)
    {
        int idx = (hash(id) + i) % array_size;

        if (bankStorage1d[idx].id == "free" || bankStorage1d[idx].id == "deleted")
        {
            bankStorage1d[idx] = acc;
            table_size++;
            load_factor = double(table_size) / array_size;
            if (load_factor > 0.7)
            {
                resize(true);
            }
            load_factor = double(table_size) / array_size;
            return;
        }
    }
}

std::vector<int> LinearProbing::getTopK(int k)
{
    std::vector<int> balances;
    for (int i = 0; i < array_size; i++)
    {
        if (bankStorage1d[i].id != "free" && bankStorage1d[i].id != "deleted")
        {
            balances.push_back(bankStorage1d[i].balance);
        }
        else
            continue;
    }
    qsort(balances, 0, balances.size() - 1);
    std::vector<int> top_k_balances;
    if (balances.size() > k)
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
        int j = balances.size() - 1;
        while (i < j)
        {
            swap(balances, i, j);
            i++;
            j--;
        }
        return balances;
    }
}

int LinearProbing::getBalance(std::string id)
{
    for (int i = 0; i < array_size; i++)
    {
        int idx = (hash(id) + i) % array_size;
        if (bankStorage1d[idx].id == id)
        {
            return bankStorage1d[idx].balance;
        }
        else if (bankStorage1d[idx].id == "free")
        {
            return -1;
        }
        else
            continue;
    }
    return -1;
}

void LinearProbing::addTransaction(std::string id, int count)
{
    for (int i = 0; i < array_size; i++)
    {
        int idx = (hash(id) + i) % array_size;
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
// Should I resize in addTransaction also?
bool LinearProbing::doesExist(std::string id)
{
    for (int i = 0; i < array_size; i++)
    {
        int idx = (hash(id) + i) % array_size;
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

bool LinearProbing::deleteAccount(std::string id)
{
    for (int i = 0; i < array_size; i++)
    {
        int idx = (hash(id) + i) % array_size;
        if (bankStorage1d[idx].id == id)
        {
            Account acc;
            acc.id = "deleted";
            acc.balance = 0;
            bankStorage1d[idx] = acc;
            table_size--;
            load_factor = double(table_size) / array_size;
            if (load_factor < 0.34 && ptr > 0)
                resize(false);
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
int LinearProbing::databaseSize()
{
    return table_size;
}

int LinearProbing::hash(std::string id)
{
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

void LinearProbing::resize(bool flag)
{
    if (flag == true)
        ptr++;
    else
        ptr--;

    ptr = std::max(0, std::min(ptr, int(prime_list.size()) - 1));

    std::vector<Account> new_array;
    new_array.resize(prime_list[ptr]);
    Account acc;
    acc.id = "free";
    acc.balance = 0;

    array_size = prime_list[ptr];

    for (int i = 0; i < array_size; i++)
    {
        // Initialize all new array slots
        new_array[i] = acc;
    }

    for (int i = 0; i < bankStorage1d.size(); i++)
    {
        if (bankStorage1d[i].id != "free" && bankStorage1d[i].id != "deleted")
        {
            int idx = hash(bankStorage1d[i].id);
            int j = 0;
            
            // Find the first available slot in the new array
            while (new_array[(idx + j) % array_size].id != "free")
            {
                j++;
            }
            
            // Copy the account to the new array
            new_array[(idx + j) % array_size] = bankStorage1d[i];
        }
    }

    bankStorage1d = new_array;
}


double LinearProbing::loadFactor()
{
    return load_factor;
}
// int main()
// {
//     LinearProbing lp;
//     std::vector<std::string> keys = {
//         "BDAC1052643_1720849563",
//         "CDAB5023614_2790643851",
//         "ACBD3614502_3418275609",
//         "BCAD5610342_2046759381",
//         "ABCD5041632_2091364857",
//         "ACBD2350416_5734860291",
//         "BCDA2561034_4081296753",
//         "DACB5016342_2596487301",
//         "ACBD6034251_9160874532",
//         "BCDA5643021_4263015798",
//         "BCDA0631542_1486307295",
//         "BADC6203415_2048635971",
//         "CBDA5102634_3218564709",
//         "DBCA6354021_0642513987",
//         "CADB3612045_2168594037",
//         "CBDA5162340_6470985231",
//         "DCBA6043152_5320967148",
//         "BDAC6501432_1948032657",
//         "CBDA0652134_6128049375",
//         "CABD3506214_6731509428",
//         "ADCB5204136_0618923457",
//         "CBAD3254160_1903724865",
//         "CBAD4236015_7350418629",
//         "CDBA1450623_5083274961",
//         "ACDB1065423_6014375982",
//         "DCBA1052634_3674901258",
//         "BADC0426513_9160375248",
//         "ADCB6451032_6713094528",
//         "DBCA4201653_7809156324",
//         "CBAD0632415_3421650978",
//         "ABCD3540126_6347201598",
//         "BCDA1462053_9201583647",
//         "BDAC3056214_1720385469",
//         "BADC0415632_8123456097",
//         "ACBD2601354_1594086327",
//         "CADB5216430_2138964507",
//         "DACB5201436_0542387196",
//         "BDAC6543201_2603517984",
//         "ACBD5436210_9716482530",
//         "ADCB0154236_4069283175",
//         "ABDC1342056_6325804197",
//         "BADC6140352_4813790652",
//         "CDAB1054623_2815430967",
//         "DABC6423051_2604137598",
//         "CDAB3421605_2198736504",
//         "ADCB6135420_4182063957",
//         "ABDC4356210_8316479250",
//         "BDCA5260134_5736028419",
//         "DABC2436150_7812540369",
//         "DACB5014632_0175328496",
//         "CDBA6501423_1950732486",
//         "BCDA3105642_6457239081",
//         "DACB5401326_3419680527",
//         "CDAB2361540_6571302894",
//         "DCAB5360241_9087136245",
//         "ABCD0143526_3067125894",
//         "CADB5046123_5748301962",
//         "ACDB3065241_5208169734",
//         "CBDA6405231_8753602149",
//         "DBCA1054632_3469571280",
//         "CBAD4213056_1792534086",
//         "CBDA2463510_5723091864",
//         "DCBA2516430_7163952048",
//         "BDAC4361205_8140723596",
//         "BADC6035142_3715248960",
//         "DBCA1462350_8309712456",
//         "CADB5314260_8790645321",
//         "DCBA6254130_1693845072",
//         "CADB2046315_5731469802",
//         "BCDA3210654_1548903672",
//         "BCDA4356021_3841507629",
//         "BDAC6512403_2045931678",
//         "BADC2516304_9804672531",
//         "ADBC5261304_3906245718",
//         "BCDA4652013_6470283195",
//     };
//     for (int i = 0; i < 71; i++)
//     {
//         lp.createAccount(keys[i], 1);
//         // std::cout<<lp.loadFactor()<<std::endl;
//     }
//     std::cout << lp.bucket_size() << std::endl;
//     std::cout << lp.databaseSize() << std::endl;

//     lp.deleteAccount("BDAC1052643_1720849563");
//     lp.deleteAccount("CDAB5023614_2790643851");

//     std::cout << lp.bucket_size() << std::endl;
//     std::cout << lp.databaseSize() << std::endl;

//     std::cout << lp.loadFactor() << std::endl;

//     lp.createAccount("BCDA4652013_6470283195", 1); // hash value = 90
//     std::cout << lp.databaseSize() << std::endl;

//     lp.deleteAccount("BCDA4652013_6470283195");
//     std::cout << lp.databaseSize() << std::endl;

//     // lp.createAccount("CDAB5023614_2790643851",1);

//     // std::cout<<lp.bucket_size()<<std::endl;
//     // std::cout<<lp.databaseSize()<<std::endl;

//     // std::cout<<lp.loadFactor()<<std::endl;

//     // std::cout<<lp.loadFactor()<<std::endl;
// }