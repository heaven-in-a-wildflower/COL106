#include "stack_b.h"
#include <iostream>
#include <stdexcept>
using namespace std;
Stack_B::Stack_B()
{
    stk = new int[1024];
    // if (!stk)
    // {
    //     throw std::runtime_error("Out of Memory");        
    // }
    size = 0;
    capacity = 1024;
}

Stack_B::~Stack_B()
{
    delete[] stk;
}

void Stack_B::push(int data)
{
    if (size < capacity)
    {
        stk[size] = data;
    }
    else
    {
        int c = 2;
        // initialise new array of double the size and copy the elements into the new array
        int* new_stk = new (std::nothrow) int[capacity * c];
        if (!new_stk)
        {
            throw std::runtime_error("Out of Memory");
        }
        for (int i = 0; i < size; i++)
        {
            new_stk[i] = stk[i];
        }
        new_stk[size] = data;
        delete[] stk;
        stk = new_stk;
        capacity *= c;
    }
    size++;
}

int Stack_B::pop()
{
    //cout << size << endl;
    if (size == 0)
    {
        throw std::runtime_error("Empty Stack");
    }
    else if (size == capacity / 4 + 1 && capacity > 1024) //i.e when size = half + 1
    {
        int c = 4;
        int* new_stk = new (std::nothrow) int[capacity / c];
        if (!new_stk)
        {
            throw std::runtime_error("Out of Memory");
        }
        int temp = stk[size-1];
        for (int i = 0; i < size - 1; i++)
        {
            new_stk[i] = stk[i];
        }
        delete[] stk;
        stk = new_stk;
        capacity /= c;
        size--;
        return temp;
    }
    else
    {
        return stk[(size--) - 1];
    }

}

int Stack_B::get_element_from_top(int idx)
{
    if (idx < 0 || idx >= size)
        throw std::runtime_error("Index out of range");
    int n = size - 1;
    return stk[n - idx];
}

int Stack_B::get_element_from_bottom(int idx)
{
    if (idx < 0 || idx >= size)
        throw std::runtime_error("Index out of range");
    return stk[idx];
}

void Stack_B::print_stack(bool top_or_bottom)
{
    if (top_or_bottom == 0)
    {
        for (int i = 0; i < size; i++)
        {
            std::cout << stk[i] << std::endl;
        }
    }
    else
    {
        for (int i = size - 1; i >= 0; i--)
        {
            std::cout << stk[i] << std::endl;
        }
    }
}

int Stack_B::add()
{
    if (size < 2)
        throw std::runtime_error("Not Enough Arguments");
    int op1 = pop();
    int op2 = pop();
    int result = op1 + op2;
    push(result);
    return result;
}

int Stack_B::subtract()
{
    if (size < 2)
        throw std::runtime_error("Not Enough Arguments");
    int op1 = pop();
    int op2 = pop();
    int result = op2 - op1;
    push(result);
    return result;
}

int Stack_B::multiply()
{
    if (size < 2)
        throw std::runtime_error("Not Enough Arguments");
    int op1 = pop();
    int op2 = pop();
    int result = op1 * op2;
    push(result);
    return result;
}

int Stack_B::divide()
{
    if (size < 2)
        throw std::runtime_error("Not Enough Arguments");
    int op1 = pop();
    int op2 = pop();
    int result;
    if (op1 > 0 && op2 >= 0)
    {
        result = op2 / op1;
        push(result);
    }
    else if (op2 != 0 && ((op1 > 0 && op2 < 0) || (op2 > 0 && op1 < 0)))
    {
        if (op2 % op1 == 0)
        {
            result = op2 / op1;
        }
        else
        {
            result = op2 / op1 - 1;
        }
        push(result);
    }
    else if (op1 < 0 and op2 <= 0)
    {
        result = (-op2) / (-op1);
        push(result);
    }
    else if (op1 == 0)
    {
        throw std::runtime_error("Divide by Zero Error");
    }
    return result;
}

int* Stack_B::get_stack()
{
    int* p = stk;
    return stk;
}

int Stack_B::get_size()
{
    return size;
}


// int main() {
//     Stack_B s;
//     // std::cout << s.get_size();
//     s.push(2);
//     // std::cout << s.get_size();
//     s.push(10);
//     s.push(-6);
//     s.push(4);
//     s.push(-15);
//     s.push(4);
//     s.push(5);
//     s.push(11);
//     s.push(-9);
//     s.push(-7);
//     cout << s.get_size() << endl;
//     cout << s.pop();
//     // cout << s.get_size();
//     // cout << s.add();
//     // s.divide();
//     // s.multiply();
//     // s.multiply();
//     //s.print_stack(0);
// }