#include "node.h"

Node::Node(int v, Node *nxt, Node *prv)
{
    value = v;
    this->next = nxt;
    this->prev = prv;
    nxt->prev = this;
    prv->next = this;
}

Node::Node(bool sentinel) {
    is_sentinel=sentinel;
}

bool Node::is_sentinel_node()
{
    return is_sentinel;
}

int Node::get_value()
{
    return this->value;
    //**
    // Should i provide value only if node is regular?
}
