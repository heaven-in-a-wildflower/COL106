/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "parser.h"
// #include "exprtreenode.cpp"
// #include "symtable.cpp"
// #include "symnode.cpp"
#include <iostream>
// #include <vector>
// Write your code below this line

// void print(vector<string>& v)
// {
//     for(int i=0;i<v.size();i++)
//     {
//         cout<<v[i]<<" ";
//     }
// }
using namespace std;
Parser::Parser()
{
    symtable = new SymbolTable;
    vector<ExprTreeNode *> expr_trees;
}

bool IsOperand(const string &token)
{
    for (char c : token)
    {
        if (!isdigit(c) && !isalpha(c) && c != '_' && c!='-')
        {
            // If the character is neither a digit nor an alphabet character, it's not an operand.
            return false;
        }
    }
    return true;
}

bool IsOperator(const string &token)
{
    // Check if the token is an operator (+, -, *, /)
    return (token.size() == 1 && (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/'));
}

vector<string> InfixToPostfix(vector<string> &tokens)
{
    vector<string> s;
    vector<string> postfix;
    for (const string &token : tokens)
    {
        if (IsOperator(token))
        {
            while (!s.empty() && s.back() != "(")
            {
                postfix.push_back(s.back());
                s.pop_back();
            }
            s.push_back(token);
        }
        else if (IsOperand(token))
        {
            postfix.push_back(token);
        }
        else if (token == "(")
        {
            s.push_back(token);
        }
        else if (token == ")")
        {
            while (!s.empty() && s.back() != "(")
            {
                postfix.push_back(s.back());
                s.pop_back();
            }
            s.pop_back(); // Pop the "("
        }
    }

    while (!s.empty())
    {
        postfix.push_back(s.back());
        s.pop_back();
    }

    return postfix;
}

bool ContainsAlphabets(const std::string &input)
{
    for (char character : input)
    {
        if (std::isalpha(character))
        {
            return true;
        }
    }
    return false;
}

ExprTreeNode *BuildExpressionTree(const vector<string> &postfixTokens, SymbolTable *symbol_table)
{
    vector<ExprTreeNode *> node_stack;
    for (const string &token : postfixTokens)
    {
        if (!IsOperator(token))
        {
            if (ContainsAlphabets(token) == false)
            {
                int value = stoi(token);
                ExprTreeNode *new_node = new ExprTreeNode("VAL", value);
                node_stack.push_back(new_node);
            }

            else
            {
                ExprTreeNode *new_node = new ExprTreeNode("VAR", symbol_table->search(token));
                new_node->id = token;
                node_stack.push_back(new_node);
            }
        }
        else
        {
            int z = 0;
            if (token == "+")
            {
                ExprTreeNode *new_node = new ExprTreeNode("ADD", z);
                new_node->right = node_stack.back();
                node_stack.pop_back();
                new_node->left = node_stack.back();
                node_stack.pop_back();
                node_stack.push_back(new_node);
            }
            else if (token == "-")
            {
                ExprTreeNode *new_node = new ExprTreeNode("SUB", z);
                new_node->right = node_stack.back();
                node_stack.pop_back();
                new_node->left = node_stack.back();
                node_stack.pop_back();
                node_stack.push_back(new_node);
            }
            else if (token == "*")
            {
                ExprTreeNode *new_node = new ExprTreeNode("MUL", z);
                new_node->right = node_stack.back();
                node_stack.pop_back();
                new_node->left = node_stack.back();
                node_stack.pop_back();
                node_stack.push_back(new_node);
            }
            else if (token == "/")
            {
                ExprTreeNode *new_node = new ExprTreeNode("DIV", z);
                new_node->right = node_stack.back();
                node_stack.pop_back();
                new_node->left = node_stack.back();
                node_stack.pop_back();
                node_stack.push_back(new_node);
            }
        }
    }
    return node_stack.back();
}

void Parser::parse(vector<string> expression)
{
    // cout << expression[0] << endl;
    if (expression[0] != "del" && expression[0] != "ret")
    {
        // Initialising the first 2 nodes
        ExprTreeNode *root = new ExprTreeNode();
        ExprTreeNode *variable = new ExprTreeNode();
        variable->type = "VAR";
        variable->id = expression[0];

        root->left = variable;
        //cout << "fuck off mahir" << endl;
        vector<string> code;
        for (int i = 2; i < expression.size(); i++)
        {
            code.push_back(expression[i]);
        }

        vector<string> postfixTokens = InfixToPostfix(code);
        ExprTreeNode *temp = BuildExpressionTree(postfixTokens, symtable);
        root->right = temp;
        expr_trees.push_back(root);

        // Update the symbol table
        int loc = symtable->search(variable->id);
        // Insert only when it is absent.
        if (loc == -2)
        {
            symtable->insert(variable->id);
            int x = symtable->search(variable->id);
            //cout << "fuck off anuj" << endl;
        }
    }
    else if (expression[0] == "del")
    {
        //cout << "here del" << endl;
        ExprTreeNode *root = new ExprTreeNode();

        ExprTreeNode *variable = new ExprTreeNode();
        variable->type = "VAR";
        variable->id = expression[2];

        ExprTreeNode *del = new ExprTreeNode();
        del->type = "DEL";

        root->left = del;
        root->right = variable;

        expr_trees.push_back(root);
        last_deleted = symtable->search(variable->id);
        // cout<<last_deleted<<endl;
        symtable->remove(variable->id);
    }

    else if (expression[0] == "ret")
    {
        //cout << "end ret" << endl;

        ExprTreeNode *root = new ExprTreeNode();
        ExprTreeNode *ret = new ExprTreeNode();
        ret->type = "RET";

        root->left = ret;

        vector<string> code;
        for (int i = 2; i < expression.size(); i++)
        {
            code.push_back(expression[i]);
        }

        vector<string> postfixTokens = InfixToPostfix(code);
        ExprTreeNode *temp = BuildExpressionTree(postfixTokens, symtable);
        root->right = temp;
        expr_trees.push_back(root);
    }
}

Parser::~Parser()
{
    for (int i = expr_trees.size() - 1; i >= 0; i--)
    {
        delete expr_trees[i];
    }
    delete symtable;
}

// int main()
// {
//     Parser p;
//     vector<string> exp1 = {"vk", ":=", "(", "(", "4", "+", "czp2c_", ")", "+", "(", "czp2c_", "+", "(", "czp2c_", "+", "czp2c_", ")", ")", ")"};
//     p.parse(exp1);
//     cout<<"done"<<endl;
// }
