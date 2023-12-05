/* Do NOT add/remove any includes statements from node header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symnode.h"

// Write your code below this line

SymNode::SymNode()
{
    height = 1;
    address = -1;
    left = NULL;
    right = NULL;
    par = NULL;
}

SymNode::SymNode(string k)
{
    key = k;
    height = 1;
    address = -1;
    left = NULL;
    right = NULL;
    par = NULL;
}

// Helper functions
int getHeight(SymNode *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return node->height;
}
int getBalanceFactor(SymNode *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

void updateHeight(SymNode *node)
{
    if (node != NULL)
    {
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        node->height = std::max(leftHeight, rightHeight) + 1;
    }
}

SymNode * SymNode::LeftLeftRotation()
{
    SymNode *newRoot = this->right;
    this->right = newRoot->left;
    if (newRoot->left != NULL)
    {
        newRoot->left->par = this;
    }
    newRoot->left = this;
    newRoot->par = this->par;
    this->par = newRoot;

    updateHeight(this);
    updateHeight(newRoot);

    return newRoot;
}

SymNode *SymNode::RightRightRotation()
{
    SymNode *newRoot = this->left;
    this->left = newRoot->right;
    if (newRoot->right != NULL)
    {
        newRoot->right->par = this;
    }
    newRoot->right = this;
    newRoot->par = this->par;
    this->par = newRoot;

    updateHeight(this);
    updateHeight(newRoot);

    return newRoot;
}

SymNode::~SymNode()
{
    left = NULL;
    right = NULL;
    par = NULL;
}
