//I Aneeket Yadav, hereby declare that I have abided by the Honour Code as mentioned on the IITD Moodle page.References used have been declared below-
//1. The C++ Programming Language by Bjarne Stroustrup(4th Deition)-Chapters-16,17,18,19,31

#include <iostream>
#include <vector>
struct Tuple
{
    bool isPresent;
    int lowerBound;
};
// The following function serves a special purpose.
// Since a searching algorithm is required Insert,BelongsTo as well as Delete functions,to minimise the code required for the sake of a more elegant solutions,I tried to build binary serach function which outputs a heterogenous data structure called Tuple,which stores a boolean value to indicate if the target element is present and an index indicator if it is present.
// By default,lowerBound has been set to vec.size() as if input is larger than all elemnts of the array,then it is appended to the vector

Tuple lowerBoundSearch(std::vector<int> &vec, int n)
{
    int l = 0;
    int u = vec.size() - 1;
    int lowerBound = vec.size();
    bool isPresent = false;
    while (l <= u)
    {
        int mid = l + (u - l) / 2;
        if (vec[mid] == n)
        {
            isPresent = true;
            lowerBound = mid;
            break;
        }
        else if (vec[mid] < n)
        {
            l = mid + 1;
        }
        else
        {
            u = mid - 1;
            lowerBound = mid;
        }
    }
    Tuple result;
    result.lowerBound = lowerBound;
    result.isPresent = isPresent;
    return result;
}

class SET
{
private:
    std::vector<int> set;

public:
    int num;
    int size;

    SET()
    {
        size = 0;
    }
    int Size() const//Since this function does not modify the state of the object,use of const keyword helps to optimise the program
    {
        return size;
    }
    void Insert(int n)
    {
        Tuple result = lowerBoundSearch(set, n);
        int j = result.lowerBound;
        if (j == set.size() || set[j] != n)
        {
            set.insert(set.begin() + j, n);
            size++;
        }
        std::cout << size << std::endl;
    }
    int Element(int i) const
    {
        return set[i];
    }
    void BelongsTo(int n)
    {
        Tuple result = lowerBoundSearch(set, n);
        if (result.isPresent == true)
        {
            std::cout << 1 << std::endl;
        }
        else
            std::cout << 0 << std::endl;
    }
    void Delete(int n)
    {
        Tuple result = lowerBoundSearch(set, n);
        int j = result.lowerBound;
        if (result.isPresent == true)
        {
            set.erase(set.begin() + j);
            size--;
        }
        std::cout << size << std::endl;
    }
    std::vector<int> GetVector() const
    {
        return set;
    }
    // The Union function is again slightly uncoventional and so is the Difference function as you will soon discover.The reason for this is as follows-
    // To optimise the program,I decided to implement the SymmetricDifference function as A△B = (A\B)U(B\A).
    // This implementation necessiates that I use the pre-defined Difference and Union functions.
    // However the assignment declared that size of set be printed after Union and Difference functions.
    // To avoid this output from within the SymmetricDifference function,I figured out a way to print size of the set only when a certain boolean variable flag assumes a false value.
    void Union(const SET& set2, bool flag)
    {
        // While the first comparison may not look algorithmically intuitive,I included it because the edge case A U A was failing prior to it.
        // This was also the sole purpose of creating the GetVector method.
        if (set2.GetVector() != this->set)
        {
            int i = 0;
            int j = 0;
            while (i < size && j < set2.Size())
            {
                if (set[i] < set2.Element(j))
                    i++;
                else if (set[i] == set2.Element(j))
                    j++;
                else
                {
                    set.insert(set.begin() + i, set2.Element(j));
                    j++;
                    size++;
                }
            }
            while (j < set2.Size())
            {
                set.push_back(set2.Element(j));
                j++;
                size++;
            }
        }
        if (flag == false)
            std::cout << size << std::endl;
    }

    void Intersection(const SET& set2)
{
    std::vector<int> new_set;
    int i = 0;
    int j = 0;

    while (i < size && j < set2.Size())
    {
        if (set[i] < set2.Element(j))
            i++;
        else if (set[i] == set2.Element(j))
        {
            new_set.push_back(set[i]);
            i++;
            j++;
        }
        else
            j++;
    }
    //Earlier I was .erase()ing every non-common element between the two sets,but later realised that it would be an unnecessarily constly due to shifting operations if done frequently.
    //So I decided to create a new_vector instead;
    //I then used the same technique in my difference method.
    set = new_set;
    size = new_set.size();
    std::cout << size << std::endl;
}

    void Difference(const SET& set2, bool flag)
{
    std::vector<int> newSet;
    int i = 0;
    int j = 0;
    
    while (i < size && j < set2.Size())
    {
        if (set[i] < set2.Element(j))
        {
            newSet.push_back(set[i]);
            i++;
        }
        else if (set[i] > set2.Element(j))
            j++;
        else
        {
            i++;
            j++;
        }
    }
    
    while (i < size)
    {
        newSet.push_back(set[i]);
        i++;
    }
    
    set = newSet;
    size = newSet.size();
    if (flag == false)
        std::cout << size << std::endl;
}
    void Symmetric_Difference(const SET& set2)
    {
        SET temp1 = *this;
        SET temp2 = set2;
        Difference(set2, true); // Note that the boolean value has been shifted to true to avoid printing anything.
        temp2.Difference(temp1, true);
        Union(temp2, true);
        std::cout << size << std::endl;
    }
    void Print() const
    {
        if (size > 0)
        {
            for (int i = 0; i < size - 1; i++)
            {
                std::cout << set[i] << ",";
            }
            std::cout << set[size - 1];
        }
        std::cout << std::endl;
    }
};
int main()
{
    std::vector<SET> Sets = {};
    int op;
    int num1;
    bool flag = false; // For the Union and Difference methods
    while (std::cin >> op >> num1)
    // Since atleast two input values are required for each command,they have been taken beforehand
    {
        if (op == 1 || op == 2 || op == 3)
        {
            // The below 3 functions have been grouped together on the basis that they involve 3 input values and only 1 set.
            int data;
            std::cin >> data;
            if (num1 >= Sets.size())
                Sets.resize(num1 + 1);
            // In the above line and the code that follows,I have used resize function of vector to avoid writing (num1-1) and (num2-1) multiple times.
            if (op == 1)
                Sets[num1].Insert(data);
            else if (op == 2)
                Sets[num1].Delete(data);
            else if (op == 3)
                Sets[num1].BelongsTo(data);
        }
        else if (op == 4 || op == 5 || op == 7 || op == 8)
        {
            // The below 4 functions have been grouped together on the basis that they involved 3 inputs in the command and involve 2 sets.
            int num2;
            std::cin >> num2;

            if (num1 >= Sets.size())
                Sets.resize(num1 + 1);
            if (num2 >= Sets.size())
                Sets.resize(num2 + 1);

            if (op == 4)
                Sets[num1].Union(Sets[num2], false);
            else if (op == 5)
                Sets[num1].Intersection(Sets[num2]);
            else if (op == 7)
                Sets[num1].Difference(Sets[num2], false);
            else if (op == 8)
                Sets[num1].Symmetric_Difference(Sets[num2]);
        }
        else if (op == 6)
        {
            if (num1 >= Sets.size())
            {
                Sets.emplace_back();
                // emplace_back() method has been used instead of push_back as it allows for an undeclared data type to be appended to the set.
                std::cout << 0 << std::endl;
            }
            else
                std::cout << Sets[num1].Size() << std::endl;
        }
        else if (op == 9)
        {
            if (num1 >= Sets.size())
                std::cout << std::endl;
            else
                Sets[num1].Print();
        }
    }
    return 0;
}