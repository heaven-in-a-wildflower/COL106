/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "exprtreenode.h"

// Write your code below this line

ExprTreeNode::ExprTreeNode()
{
  // val must be the evaluated value of the entire parsed expression
  // Its left subtree would contain the variable and right subtree would evaluate to the required value
  type = ":=";
  left = NULL;
  right = NULL;
}

ExprTreeNode::ExprTreeNode(string t, int v)
{
  type = t;
  num = v;
  left = NULL;
  right = NULL;
}

ExprTreeNode::~ExprTreeNode()
{
  left = NULL;
  right = NULL;
}