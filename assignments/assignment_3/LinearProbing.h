#ifndef LINEARPROBING_H
#define LINEARPROBING_H

#include "BaseClass.h"
#include <iostream>

class LinearProbing : public BaseClass {
public:
    int table_size = 0;
    int ptr = 0;
    std::vector<int> prime_list = {100003,200003,400009};
    int array_size = prime_list[ptr];
    double load_factor = double(table_size) / array_size;
    std::vector<Account> bankStorage1d;
    
    LinearProbing();
    void createAccount(std::string id, int count) override;
    std::vector<int> getTopK(int k) override;
    int getBalance(std::string id) override;
    void addTransaction(std::string id, int count) override;
    bool doesExist(std::string id) override;
    bool deleteAccount(std::string id) override;
    int databaseSize() override;
    int hash(std::string id) override;
    void resize(bool flag);
    double loadFactor();
    
private:
    // Other data members and functions specific to Linear Probing
};

#endif // LINEARPROBING_H
