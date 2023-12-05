#ifndef CHAINING_H
#define CHAINING_H

#include "BaseClass.h"
#include <iostream>
#include <vector>

class Chaining : public BaseClass
{
public:
    int table_size = 0;
    int ptr = 0;
    std::vector<int> prime_list = {100003,200003,400009};
    int bucket_array_size = prime_list[ptr];
    double load_factor = double(table_size) / bucket_array_size;
    std::vector<std::vector<Account>> bankStorage2d;
    
    Chaining();
    void createAccount(std::string id, int count) override;
    std::vector<int> getTopK(int k) override;
    int getBalance(std::string id) override;
    void addTransaction(std::string id, int count) override;
    bool doesExist(std::string id) override;
    bool deleteAccount(std::string id) override;
    int databaseSize() override;
    int hash(std::string id) override;
    double loadFactor();
    void resize(bool flag);

private:
    // Other data members and functions specific to Chaining
};

#endif // CHAINING_H
