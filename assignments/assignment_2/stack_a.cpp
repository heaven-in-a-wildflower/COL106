#include "stack_a.h"
#include <iostream>  //**
#include <stdexcept> //**
/* PART A */
/* Stacks using Fixed Size Arrays */

Stack_A::Stack_A()
    : size(0) {}

void Stack_A::push(int data)
{
    if (size == 1024)
        throw std::runtime_error("Stack Full");
    stk[size] = data;
    size++;
}

int Stack_A::pop()
{
    if (size == 0)
        throw std::runtime_error("Empty Stack");
    size--;
    return stk[size];
}

int Stack_A::get_element_from_top(int idx)
{
    if (idx < 0 || idx >= size)
        throw std::runtime_error("Index out of range");
    return stk[size - idx-1];
}

int Stack_A::get_element_from_bottom(int idx)
{
    if (idx < 0 || idx >= size)
        throw std::runtime_error("Index out of range");
    return stk[idx];
}

void Stack_A::print_stack(bool top)
{
    if (top == 0)
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

int Stack_A::add()
{
    if (size < 2)
        throw std::runtime_error("Not Enough Arguments");
    int op1 = pop();
    int op2 = pop();
    int result = op1 + op2;
    this->push(result);
    return result;
}

int Stack_A::subtract()
{
    if (size < 2)
        throw std::runtime_error("Not Enough Arguments");
    int op1 = pop();
    int op2 = pop();
    int result = op2 - op1;
    push(result);
    return result;
}

int Stack_A::multiply()
{
    if (size < 2)
        throw std::runtime_error("Not Enough Arguments");
    int op1 = pop();
    int op2 = pop();
    int result = op1 * op2;
    push(result);
    return result;
}

int Stack_A::divide()
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
    else if(op1<0 and op2<=0)
    {
        result = (-op2)/(-op1);
        push(result);
    }
    else if (op1 == 0)
    {
        throw std::runtime_error("Divide by Zero Error");
    }
    return result;
}

int *Stack_A::get_stack() // Get a pointer to the array
{
    int *p = stk;
    return p;
}

int Stack_A::get_size() // Get the size of the stack
{
    return size;
}

//     int main(){
//     Stack_A s;
//     s.push(2);
//     s.push(10);
//     s.push(-6);
//     s.push(4);
//     s.push(-15);
//     s.push(4);
//     s.push(5);
//     s.push(11);
//     s.push(-9);
//     s.push(-7);
//     s.add();
//     s.add();
//     s.add();
//     s.subtract();
//     s.subtract();
//     s.divide();
//     // s.divide();
//     // s.multiply();
//     // s.multiply();
//     s.print_stack(0);
// }