/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "exprtreenode.h"

// '=' sign must be present at the root.
// Default constructor is probably meant for that.
ExprTreeNode::ExprTreeNode()
{
    // val must be the evaluated value of the entire parsed expression
    // Its left subtree would contain the variable and right subtree would evaluate to the required value
    id = "";
    left = nullptr;
    right = nullptr;
}

ExprTreeNode::ExprTreeNode(string t, UnlimitedRational *v)
{
    type = t;
    evaluated_value = v;
    val = v;
    left = nullptr;
    right = nullptr;
}

ExprTreeNode::ExprTreeNode(string t, UnlimitedInt *v)
{
    // Int value must be converted to rational for operations with other rationals
    type = t;
    UnlimitedInt *pseudo_den = new UnlimitedInt("1");
    UnlimitedRational *r = new UnlimitedRational(v, pseudo_den);
    evaluated_value = r;
    val = r;
    delete pseudo_den;
}

ExprTreeNode::~ExprTreeNode()
{
    delete evaluated_value;
    left = nullptr;
    right = nullptr;
}