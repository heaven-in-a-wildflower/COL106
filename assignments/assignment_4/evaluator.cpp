/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "evaluator.h"

#include <iostream>
//#include <vector>
//#include <string>
//#include "ulimitedint.cpp"
//#include "ulimitedrational.cpp"
//#include "entry.cpp"
//#include "symtable.cpp"
//#include "exprtreenode.cpp"

using namespace std;

class StringStack
{
private:
    vector<string> stack;

public:
    StringStack() {}

    bool isEmpty()
    {
        return stack.empty();
    }

    void push(const string &value)
    {
        stack.push_back(value);
    }

    string pop()
    {
        if (isEmpty())
        {
            return "";
        }
        string value = stack.back();
        stack.pop_back();
        return value;
    }

    string peek()
    {
        if (isEmpty())
        {
            return "";
        }
        return stack.back();
    }
};

class NodeStack
{
private:
	vector<ExprTreeNode *> stack;
public:
    NodeStack() {}
    bool isEmpty()
    {
        return stack.empty();
    }

    void push(ExprTreeNode *node)
    {
        stack.push_back(node);
    }

    ExprTreeNode *pop()
    {
        if (isEmpty())
        {
            return nullptr;
        }
        ExprTreeNode *node = stack.back();
        stack.pop_back();
        return node;
    }

    ExprTreeNode *peek()
    {
        if (isEmpty())
        {
            return nullptr;
        }
        return stack.back();
    }
	
	vector<ExprTreeNode *> get_vector()
	{
		return stack;
	}

};

bool IsOperand(const string &token)
{
    for (char c : token)
    {
        if (!isdigit(c) && !isalpha(c))
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

vector<string> InfixToPostfix(const vector<string> &tokens, StringStack &S)
{
    vector<string> postfix;
    for (const string &token : tokens)
    {
        if (IsOperator(token))
        {
            while (!S.isEmpty() && S.peek() != "(")
            {
                postfix.push_back(S.pop());
            }
            S.push(token);
        }
        else if (IsOperand(token))
        {
            postfix.push_back(token);
        }
        else if (token == "(")
        {
            S.push(token);
        }
        else if (token == ")")
        {
            while (!S.isEmpty() && S.peek() != "(")
            {
                postfix.push_back(S.pop());
            }
            S.pop(); // Pop the "("
        }
    }

    while (!S.isEmpty())
    {
        postfix.push_back(S.pop());
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
    // Note that the tree is being built bottom up.So it is possible to do evaluation simultaneiously.
    NodeStack node_stack;
    for (const string &token : postfixTokens)
    {
        if (!IsOperator(token))
        {
            // If the token is an operand, push it onto the stack as a leaf node
            if (ContainsAlphabets(token) == false)
		{
	    //std::cout << "pikachu9" << std::endl;
	    UnlimitedInt *value = new UnlimitedInt(token);
	    ExprTreeNode *new_node = new ExprTreeNode("VAL", value);
	    node_stack.push(new_node);
	    //delete value; // This line deletes 'value', but not 'new_node'
		}

            else
            {
                //std::cout<<"pikachu6"<<std::endl;
                // search for value of variable in symbol table
               // std::cout<<"omg"<<std::endl;
              
                //std::cout<<value->get_frac_str()<<std::endl;
                
		ExprTreeNode *new_node = new ExprTreeNode("VAR", symbol_table->search(token));
                
		new_node->id = token;
                node_stack.push(new_node);
		
		//std::cout<<node_stack.peek()->evaluated_value->get_frac_str()<<std::endl;

            }
        }
        else
        {
            // Just a trivial value to initialise the following expr nodes because alternate constructor was not available
            UnlimitedInt *x = new UnlimitedInt("0");
            UnlimitedInt *y = new UnlimitedInt("1");
            UnlimitedRational *z = new UnlimitedRational(x, y);

            if (token == "+")
            {
                //std::cout<<"pikachu10"<<std::endl;
                ExprTreeNode *new_node = new ExprTreeNode("ADD", z);

		//std::cout<<node_stack.peek()->evaluated_value->get_frac_str()<<std::endl;                
		//std::cout<<node_stack.get_vector().size()<<std::endl;
		//std::cout<<node_stack.get_vector()[0]->evaluated_value->get_frac_str()<<std::endl;
                new_node->right = node_stack.pop();

		//std::cout<<node_stack.peek()->evaluated_value->get_frac_str()<<std::endl;

                new_node->left = node_stack.pop();

		//std::cout<<node_stack.isEmpty()<<std::endl;		

		//std::cout<<(new_node->left == nullptr)<<std::endl;                
                //std::cout<<"pikachu11"<<std::endl;
                
		
		UnlimitedRational *left_eval = new_node->left->evaluated_value;
		
		//Problem is that left_eval is not being stored properly
 		
		//std::cout<<left_eval->get_frac_str()<<std::endl; //This line will give segmentation fault 
 
               UnlimitedRational *right_eval = new_node->right->evaluated_value; 
                
		//std::cout<<right_eval->get_frac_str()<<std::endl;

                //std::cout<<"pikachu12"<<std::endl;
                
		new_node->evaluated_value = UnlimitedRational::add(left_eval, right_eval); 

               // std::cout<<"pikachu13"<<std::endl;
                node_stack.push(new_node);
                //delete left_eval;
                //delete right_eval;
            }
            else if (token == "-")
            {
                ExprTreeNode *new_node = new ExprTreeNode("SUB", z);
                new_node->right = node_stack.pop();
                new_node->left = node_stack.pop();
                UnlimitedRational *left_eval = new_node->left->evaluated_value;
                UnlimitedRational *right_eval = new_node->right->evaluated_value;
                new_node->evaluated_value = UnlimitedRational::sub(left_eval, right_eval);
                node_stack.push(new_node);
                //delete left_eval;
                //delete right_eval;
            }
            else if (token == "*")
            {
                ExprTreeNode *new_node = new ExprTreeNode("MUL", z);
                new_node->right = node_stack.pop();
                new_node->left = node_stack.pop();
                UnlimitedRational *left_eval = new_node->left->evaluated_value;
                UnlimitedRational *right_eval = new_node->right->evaluated_value;
                new_node->evaluated_value = UnlimitedRational::mul(left_eval, right_eval);
                node_stack.push(new_node);
                //delete left_eval;
                //delete right_eval;
            }
            else if (token == "/")
            {
                ExprTreeNode *new_node = new ExprTreeNode("DIV", z);
                new_node->right = node_stack.pop();
                new_node->left = node_stack.pop();
                UnlimitedRational *left_eval = new_node->left->evaluated_value;
                UnlimitedRational *right_eval = new_node->right->evaluated_value;
                new_node->evaluated_value = UnlimitedRational::div(left_eval, right_eval);
                node_stack.push(new_node);
                //delete left_eval;
                //delete right_eval;
            }
        }
    }
    return node_stack.pop();
}

Evaluator::Evaluator()
{
    symtable = new SymbolTable();
    vector<ExprTreeNode *> vec;
    expr_trees = vec;
}

Evaluator::~Evaluator()
{
//    for (int i = expr_trees.size()-1;i>=0; i--)
//    {
//	std::cout<<expr_trees[i]->left->id<<std::endl;
//        delete expr_trees[i]; 
//    }
    expr_trees.clear();
    //delete symtable;
}

void Evaluator::parse(const vector<string> code)
{
    // Initialising the first 2 nodes
    ExprTreeNode *root = new ExprTreeNode();
    ExprTreeNode *variable = new ExprTreeNode();
    variable->type = "VAR";
    variable->id = code[0];
    //std::cout<<code[0]<<std::endl;
    root->left = variable;
    //std::cout<<"pikachu3"<<std::endl;
    //code.erase(code.begin(), code.begin() + 2);
    vector<string> code1;
    int n = code.size();
    for(int i = 2;i<n;i++){code1.push_back(code[i]);}
    StringStack s;
    vector<string> postfixTokens = InfixToPostfix(code1, s);
   // std::cout<<"pikachu4"<<std::endl;
   // std::cout << std::endl;
    // Now we have to build the parse tree from the postfix expression
    ExprTreeNode *temp = BuildExpressionTree(postfixTokens, symtable);
	//clear the stack. 
    //std::cout<<"pikachu5"<<std::endl;
    // Evaluation has also been done simultaneously.
    root->right = temp;
   // std::cout<<temp->evaluated_value->get_frac_str()<<std::endl;
//cout<<root->left->id<<endl;
    expr_trees.push_back(root);
}

void Evaluator::eval()
{
    ExprTreeNode *root = expr_trees.back();
    //std::cout<<"pikachu7"<<std::endl;
    symtable->insert(root->left->id, root->right->evaluated_value);
   // std::cout<<root->right->evaluated_value->get_frac_str()<<std::endl; 
   // std::cout<<"pikachu8"<<std::endl;
}


//int main()
//{
//    Evaluator *tree = new Evaluator();
//
//    // Expression 1: x := (2+(3*4))
//    vector<string> v = {"x", ":=", "(", "2", "+", "(", "3", "*", "4", ")", ")"}; 
//    StringStack s1;
//    std::vector<string> postfix = InfixToPostfix(v, s1);
//
//    tree->parse(v);
//    tree->eval();
//    UnlimitedRational *val = tree->symtable->search("x");
//    cout << "Value of x: " << val->get_frac_str() << endl;
//	cout << "Value of x: " << val->get_frac_str() << endl;
////cout << "Value of x: " << val->get_p_str() << endl;
//	std::cout<<tree->expr_trees.back()->right->left->evaluated_value->get_frac_str()<<std::endl;
//    //Expression 2: v := (x+3)
//    vector<string> v1 = {"v", ":=", "(", "x", "+", "3", ")"}; 
//    StringStack s2;
//    postfix = InfixToPostfix(v1, s2);
//
//	//std::cout<<std::endl;
//    //std::cout<<"pikachu1"<<std::endl;
//    tree->parse(v1);
//   // std::cout<<"pikachu2"<<std::endl;
//    tree->eval();
//    UnlimitedRational *val1 = tree->symtable->search("v");
//    cout << "Value of v: " << val1->get_frac_str() << endl;
//
//    delete tree;
//}

