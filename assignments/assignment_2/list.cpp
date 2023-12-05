#include "list.h"
#include <iostream>
#include<stdexcept>

List::List()
{
    sentinel_head = new (std::nothrow) Node();
    if (!sentinel_head)
    {
        throw std::runtime_error("Out of Memory");
    }
    sentinel_tail = new (std::nothrow) Node();
    if (!sentinel_tail)
    {
        throw std::runtime_error("Out of Memory");
    }
    sentinel_head->next = sentinel_tail;
    sentinel_tail->prev = sentinel_head;
    // Should I add //sentinel_head->prev = nullptr;
    sentinel_tail->next = nullptr;
    size = 0;
}

List::~List()
{
    Node *current = sentinel_head;
    while (current != nullptr)
    {
        Node *temp = current;
        current = current->next;
        delete temp;
    }
    sentinel_head = nullptr;
}

void List::insert(int v)
{
    Node *temp1 = sentinel_tail->prev;
    Node *temp2 = new (std::nothrow) Node(v, sentinel_tail, temp1);
    if (temp2 == nullptr)
    { 
        throw std::runtime_error("Out of Memory");
    }
    size++;
}

int List::delete_tail()
{
    Node *temp = (sentinel_tail->prev);
    int data = temp->get_value();
    (temp->prev)->next = sentinel_tail;
    sentinel_tail->prev = temp->prev;
    size--;
    delete temp;
    return data;
}

int List::get_size()
{
    return size;
}

Node *List::get_head()
{
    return sentinel_head;
}