/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symtable.h"
// #include "symnode.cpp"
// #include <iostream>
// #include <vector>

// Write your code below this line

SymbolTable::SymbolTable()
{
    root = NULL;
    size = 0;
}

SymNode *SymbolTable::get_root()
{
    return root;
}

int SymbolTable::get_size()
{
    return size;
}

// Helper functions
int getHeight_(SymNode *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return node->height;
}
int getBalanceFactor_(SymNode *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return getHeight_(node->left) - getHeight_(node->right);
}

void updateHeight_(SymNode *node)
{
    if (node != NULL)
    {
        int leftHeight = getHeight_(node->left);
        int rightHeight = getHeight_(node->right);
        node->height = std::max(leftHeight, rightHeight) + 1;
    }
}

SymNode *balance(SymNode *node)
{
    updateHeight_(node);
    int balance = getBalanceFactor_(node);

    if (balance > 1)
    {
        if (getBalanceFactor_(node->left) < 0)
        {
            node->left = node->left->LeftLeftRotation();
        }
        return node->RightRightRotation();
    }
    if (balance < -1)
    {
        if (getBalanceFactor_(node->right) > 0)
        {
            node->right = node->right->RightRightRotation();
        }
        return node->LeftLeftRotation();
    }
    return node;
}

SymNode *insertUtil(SymNode *node, string key, SymNode *parent = NULL)
{
    if (node == NULL)
    {
        return new SymNode(key);
    }
    else if (key < node->key)
    {
        node->left = insertUtil(node->left, key, node);
    }
    else if (key > node->key)
    {
        node->right = insertUtil(node->right, key, node);
    }

    return balance(node);
}

SymNode *findMinNode(SymNode *node)
{
    while (node->left != NULL)
    {
        node = node->left;
    }
    return node;
}

SymNode *deleteUtil(SymNode *node, string key)
{
    if (node == NULL)
    {
        return node;
    }

    if (key < node->key)
    {
        node->left = deleteUtil(node->left, key);
    }
    else if (key > node->key)
    {
        node->right = deleteUtil(node->right, key);
    }
    else
    {
        // Node with only one child or no child
        if (node->left == NULL || node->right == NULL)
        {
            SymNode *temp = node->left ? node->left : node->right;
            if (temp == NULL)
            {
                temp = node;
                node = NULL;
            }
            else
            {
                *node = *temp;
            }
            delete temp;
        }
        else
        {
            // Node with two children: Get the inorder successor (smallest in the right subtree)
            SymNode *successor = findMinNode(node->right);
            node->key = successor->key;
            node->address = successor->address;
            node->right = deleteUtil(node->right, successor->key);
        }
    }

    if (node == NULL)
    {
        return node;
    }

    return balance(node);
}
// Methods
void SymbolTable::insert(string k)
{
    root = insertUtil(root, k);
}

void SymbolTable::remove(string key)
{
    root = deleteUtil(root, key);
}

int SymbolTable::search(string k)
{
    if (root != NULL)
    {
        SymNode *current = root;
        while (current != NULL)
        {
            if (k < current->key)
            {
                current = current->left;
            }
            else if (k > current->key)
            {
                current = current->right;
            }
            else
            {
                return current->address;
            }
        }
    }
    return -2;
}

void SymbolTable::assign_address(string k, int idx)
{
    if (root != NULL)
    {
        SymNode *current = root;
        while (current != NULL)
        {
            if (k < current->key)
            {
                current = current->left;
            }
            else if (k > current->key)
            {
                current = current->right;
            }
            else
            {
                current->address = idx;
                break;
            }
        }
    }
}

void destroyTree(SymNode *node)
{
    if (node != NULL)
    {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}
SymbolTable::~SymbolTable()
{
    destroyTree(this->get_root());
}

// void preOrderTraversal(SymNode *node)
// {
//     if (node != NULL)
//     {
//         cout << node->key << " ";
//         preOrderTraversal(node->left);
//         preOrderTraversal(node->right);
//     }
// }

// void inOrderTraversal(SymNode *node)
// {
//     if (node != NULL)
//     {
//         inOrderTraversal(node->left);
//         cout << node->key << " ";
//         inOrderTraversal(node->right);
//     }
// }

// int main()
// {
//     SymbolTable* t = new SymbolTable();
//     t->insert("a");
//     t->assign_address("a",0);
//     t->insert("b");
//     t->assign_address("b",1);
//     t->insert("c");
//     t->assign_address("c",2);
//     cout<<t->search("c")<<endl;
    
//     cout<<t->search("b")<<endl;
//     t->remove("b");

//     cout<<t->search("c")<<endl;
// }
