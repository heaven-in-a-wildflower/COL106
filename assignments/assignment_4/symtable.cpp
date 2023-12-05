/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
// #include <iostream>
// #include <string>
// #include <vector>
#include "symtable.h"
// #include "entry.cpp"
// #include "ulimitedrational.cpp"
// #include "ulimitedint.cpp"

SymbolTable::SymbolTable()
{
    SymEntry *root = new SymEntry();
}

void destroyTable(SymEntry *node)
{
    if (node != nullptr)
    {
        destroyTable(node->left);
        destroyTable(node->right);
        delete node;
    }
}

SymbolTable::~SymbolTable()
{
    destroyTable(root);
}

void SymbolTable::insert(string k, UnlimitedRational *v)
{
    SymEntry *new_node = new SymEntry(k, v);

    if (root == nullptr)
    {
        root = new_node;
        size = 1;
    }
    else
    {
        SymEntry *current = root;
        SymEntry *parent = nullptr;

        while (current != nullptr)
        {
            parent = current;
            if (k < current->key)
                current = current->left;
            else
                current = current->right;
        }

        if (k < parent->key)
            parent->left = new_node;
        else
            parent->right = new_node;
        size++;
    }
}
void SymbolTable::remove(string k)
{
    if (root == nullptr)
    {
        return;
    }

    SymEntry *parent = nullptr;
    SymEntry *current = root;

    // Search for node with given key
    while (current != nullptr)
    {
        if (k == current->key)
        {
            break;
        }
        parent = current;
        if (k < current->key)
            current = current->left;
        else
            current = current->right;
    }

    if (current == nullptr)
    {
        // Key not found
        return;
    }

    // Node has been found

    // Case 1: Node has no children
    if (current->left == nullptr && current->right == nullptr)
    {
        if (current == root)
        {
            root = nullptr;
        }
        else if (current == parent->left)
        {
            parent->left = nullptr;
        }
        else
        {
            parent->right = nullptr;
        }
        delete current;
    }

    // Case 2: Node has only right child
    else if (current->left == nullptr)
    {
        if (current == root)
        {
            root = current->right;
        }
        else if (current == parent->left)
        {
            parent->left = current->right;
        }
        else
        {
            parent->right = current->right;
        }
        delete current;
    }

    // Case 3: Node has only left child
    else if (current->right == nullptr)
    {
        if (current == root)
        {
            root = current->left;
        }
        else if (current == parent->left)
        {
            parent->left = current->left;
        }
        else
        {
            parent->right = current->left;
        }
        delete current;
    }

    // Case 4: Node has 2 children
    else
    {
        // Find successor: leftmost node in right subtree
        SymEntry *succ = current->right;
        SymEntry *succ_parent = current;
        while (succ->left != nullptr)
        {
            succ_parent = succ;
            succ = succ->left;
        }

        // Copy succ's key value to current
        current->key = succ->key;
        current->val = succ->val;

        // Remove succ, note that it will have at most 1 right child
        if (succ_parent->left == succ)
        {
            succ_parent->left = succ->right;
        }
        else
        {
            succ_parent->right = succ->right;
        }
        delete succ;
    }
    size--;
}


UnlimitedRational *SymbolTable::search(string k)
{
    SymEntry *current = root;
    while (current != nullptr)
    {
        if (k == current->key)
            return current->val;
        else if (k < current->key)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

// void postorder(SymEntry *node, vector<string> &v)
// {
//     if (node == nullptr)
//         return;
//     postorder(node->left, v);
//     postorder(node->right, v);
//     v.push_back(node->key);
// }

int SymbolTable::get_size()
{
    return size;
}

SymEntry *SymbolTable::get_root()
{
    return root;
}

// int main() {
//     UnlimitedInt* a = new UnlimitedInt("1");
//     UnlimitedRational* b = new UnlimitedRational(a, a);
//     SymbolTable* table = new SymbolTable();


//     table->insert("e", b);
//     table->insert("c", b);
//     table->insert("a", b);
//     table->insert("d", b);
//     table->insert("b", b);
//     table->insert("h", b);
//     table->insert("f", b);
//     table->insert("i", b);
//     table->insert("g", b);
//     table->insert("k", b);
//     table->insert("j", b);

//     SymEntry* root = table->get_root();
    
//     // cout << "Size: " << table->get_size() << endl;
    
//     table->remove("g");
//     vector<string> res;
//     postorder(root, res);
//     cout << "Postorder Traversal: ";
//     for (int i = 0; i < res.size(); i++) {
//         cout << res[i] << " ";
//     }
//     cout << endl;

//     return 0;
// }

