#include "stack_c.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Stack_C::Stack_C() {stk=new List();}

Stack_C::~Stack_C() { delete stk; }

void Stack_C::push(int data)
{
    stk->insert(data);
}

int Stack_C::pop()
{
    int n = stk->get_size();
    if (n <= 0)
        throw std::runtime_error("Empty Stack");
    return stk->delete_tail();
}

int Stack_C::get_element_from_top(int idx)
{
    if (idx >= stk->get_size() || idx < 0)
        throw std::runtime_error("Index out of range");
    Node *temp = stk->get_head();
    int n = stk->get_size();
    for (int i = 0; i < n - idx; i++)
    {
        temp = temp->next;
    }
    return temp->get_value();
}

int Stack_C::get_element_from_bottom(int idx)
{
    if (idx >= stk->get_size() || idx < 0)
        throw std::runtime_error("Index out of range");
    Node *temp = stk->get_head();
    for (int i = 0; i <= idx; i++)
    {
        temp = temp->next;
    }
    return temp->get_value();
}

void Stack_C::print_stack(bool top)
{

    Node *temp = (stk->get_head())->next;
    int n = stk->get_size();
    if (n >= 1)
    {
        if (top == 0)
        {
            for (int i = 1; i <= n; i++)
            {
                std::cout << temp->get_value();
                temp = temp->next;
            }
        }
        else
        {
            for (int i = 1; i <= n - 1; i++)
            {
                temp = temp->next;
            }
            for (int j = n; j > 1; j--)
            {
                std::cout << temp->get_value();
                temp = temp->prev;
            }
        }
    }
}

int Stack_C::add()
{
    if (stk->get_size() < 2)
        throw std::runtime_error("Not Enough Arguments");
    int op1 = pop();
    int op2 = pop();
    int result = op1 + op2;
    push(result);
    return result;
}

int Stack_C::subtract()
{
    if (stk->get_size() < 2)
        throw std::runtime_error("Not Enough Arguments");
    int op1 = pop();
    int op2 = pop();
    int result = op2 - op1;
    push(result);
    return result;
}

int Stack_C::multiply()
{
    if (stk->get_size() < 2)
        throw std::runtime_error("Not Enough Arguments");
    int op1 = pop();
    int op2 = pop();
    int result = op2 * op1;
    push(result);
    return result;
}

int Stack_C::divide()
{
    if (stk->get_size() < 2)
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

List *Stack_C::get_stack()
{
    List *p = stk;
    return p;
}

int Stack_C::get_size()
{
    return stk->get_size();
}

// int main() {
//     Stack_C s;
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
//     // cout << s.pop();
//     // cout << s.get_size();
//     // cout << s.add();
//     // s.divide();
//     // s.multiply();
//     // s.multiply();
//     // s.print_stack(0);
// }