/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "minheap.h"
// #include "heapnode.cpp"
// #include <iostream>
using namespace std;

// #include <queue>

// void bfsTraversal(HeapNode *root)
// {
//     if (!root)
//     {
//         std::cout << "Tree is empty." << std::endl;
//         return;
//     }

//     std::queue<HeapNode *> q;
//     q.push(root);

//     while (!q.empty())
//     {
//         HeapNode *node = q.front();
//         q.pop();

//         if (node)
//         {
//             std::cout << node->val << " ";
//             q.push(node->left);
//             q.push(node->right);
//         }
//         else
//         {
//             std::cout << "NULL ";
//         }
//     }
//     std::cout << std::endl;
// }
// Write your code below this line

MinHeap::MinHeap()
{
    size = 0;
    root = NULL;
}

int height(HeapNode *root)
{
    if (root == NULL)
    {
        return 0;
    }

    int leftHeight = height(root->left);
    int rightHeight = height(root->right);

    return std::max(leftHeight, rightHeight) + 1;
}

void findRightMostLowest(HeapNode *root, int currentLevel, int &maxLevel, HeapNode *&result)
{
    if (root == NULL)
    {
        return;
    }

    // If we reach a new level, update the rightmost element
    if (currentLevel > maxLevel)
    {
        maxLevel = currentLevel;
        result = root;
    }

    // Recursively visit right child first (DFS)
    findRightMostLowest(root->right, currentLevel + 1, maxLevel, result);
    findRightMostLowest(root->left, currentLevel + 1, maxLevel, result);
}

void heapifyUp(HeapNode *node)
{
    if (node->par == NULL)
    {
        return;
    }
    else
    {
        if (node->par->val > node->val)
        {
            swap(node->val, node->par->val); // inline void std::swap<int>(int &, int &) noexcept(true)
        }
        heapifyUp(node->par);
    }
}

void MinHeap::push_heap(int x)
{
    HeapNode *new_node = new HeapNode(x);
    if (root == NULL)
    {
        root = new_node;
        return;
    }

    // Find insertion position
    int maxLevel = -1;
    HeapNode *temp = NULL;
    findRightMostLowest(root, 0, maxLevel, temp);

    // Case 1
    if (temp == root)
    {
        if (root->left == NULL)
        {
            root->left = new_node;
        }
        else
        {
            root->right = new_node;
        }
        new_node->par = root;
    }

    // Case 2
    else if (temp == temp->par->left)
    {
        temp->par->right = new_node;
        new_node->par = temp->par;
    }

    // Case 3
    else
    {
        while (temp != root && temp == temp->par->right) // Witness the horrors of short-circuit evaluation by changing the order of operands
        {
            temp = temp->par;
        }

        if (temp != root)
        {
            temp = temp->par->right;
            while (temp->left != NULL)
            {
                temp = temp->left;
            }
            temp->left = new_node;
            new_node->par = temp;
        }
        else
        {
            while (temp->left != NULL)
            {
                temp = temp->left;
            }
            temp->left = new_node;
            new_node->par = temp;
        }
    }
    // Structural property is now satisfied.Now to satisfy the heap property by heapifyUp
    heapifyUp(new_node);
}

int MinHeap::get_min()
{
    return root->val;
}

void heapifyDown(HeapNode *node)
{
    if (node->left == NULL and node->right == NULL)
    {
        return;
    }
    HeapNode *temp = node;
    if (node->val > node->left->val)
    {
        temp = node->left;
    }
    if (node->right != NULL && temp->val > node->right->val)
    {
        temp = node->right;
    }
    if (temp != node)
    {
        swap(temp->val, node->val);
        heapifyDown(temp);
    }
}
void MinHeap::pop()
{
    if (root == NULL)
    {
        return;
    }

    // Find end position
    int maxLevel = -1;
    HeapNode *temp = NULL;
    findRightMostLowest(root, 0, maxLevel, temp);

    if (root == temp)
    {
        // If the root is the only element in the heap
        delete root;
        root = NULL;
    }
    else
    {
        root->val = temp->val;
        if (temp == temp->par->left)
        {
            temp->par->left = NULL;
        }
        else if (temp == temp->par->right)
        {
            temp->par->right = NULL;
        }
        delete temp;
        heapifyDown(root);
    }
}


void destroyTree(HeapNode *node)
{
    if (node != NULL)
    {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

MinHeap::~MinHeap()
{
    destroyTree(root);
}

// void preOrderTraversal(HeapNode *node)
// {
//     if (node != NULL)
//     {
//         cout << node->val << " ";
//         preOrderTraversal(node->left);
//         preOrderTraversal(node->right);
//     }
// }

// void inOrderTraversal(HeapNode *node)
// {
//     if (node != NULL)
//     {
//         inOrderTraversal(node->left);
//         cout << node->val << " ";
//         inOrderTraversal(node->right);
//     }
// }

// HeapNode *MinHeap::get_root()
// {
//     return root;
// }

// int main()
// {
//     MinHeap *x = new MinHeap();
//     for (int i = 0; i < 5; i++)
//     {
//         x->push_heap(i);
//     }
//     for (int i = 0; i < 5; i++)
//     {
//         x->pop();
//         cout << "sam" << endl;
//     }
//     cout << "boy" << endl;
//     for (int i = 0; i < 5; i++)
//     {
//         x->push_heap(i);
//     }
//     bfsTraversal(x->root);
// }