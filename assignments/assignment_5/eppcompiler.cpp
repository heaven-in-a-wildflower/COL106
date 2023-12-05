/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "eppcompiler.h"
// #include "parser.cpp"
// #include "minheap.cpp"
// #include "heapnode.cpp"
// #include "symtable.cpp"
// #include "symnode.cpp"
// #include "exprtreenode.cpp"
// #include <iostream>
using namespace std;
// Write your code below this line

// void print(vector<string> &v)
// {
//     for (int i = 0; i < v.size(); i++)
//     {
//         cout << v[i] << endl;
//     }
// }

EPPCompiler::EPPCompiler()
{
    output_file = "";
    MinHeap least_mem_loc;
}

EPPCompiler::EPPCompiler(string out_file, int mem_limit)
{
    output_file = out_file;
    memory_size = mem_limit;

    for (int i = 0; i < mem_limit; i++)
    {
        least_mem_loc.push_heap(i);
    }
    std::ofstream of1;
    of1.open(output_file, std::ofstream::out | std::ofstream::trunc);
    of1.close();
}

void EPPCompiler::compile(vector<vector<string>> code)
{
    // cout<<"my my"<<endl;
    for (int i = 0; i < code.size(); i++)
    {
        targ.parse(code[i]);
        vector<string> commands = generate_targ_commands();
        write_to_file(commands);
    }
}

void reverse(vector<string> &v)
{
    int i = 0;
    int j = v.size() - 1;
    while (i < j)
    {
        string temp = v[i];
        v[i] = v[j];
        v[j] = temp;
        i++;
        j--;
    }
}

vector<string> EPPCompiler::generate_targ_commands()
{
    vector<string> commands;
    vector<ExprTreeNode *> nodeStack;

    nodeStack.push_back(this->targ.expr_trees.back());
    while (!nodeStack.empty())
    {
        ExprTreeNode *currentNode = nodeStack.back();
        nodeStack.pop_back();

        if (currentNode->type == "VAL")
        {
            commands.push_back("PUSH " + to_string(currentNode->num));
        }

        else if (currentNode->type == "VAR")
        {
            int loc = targ.symtable->search(currentNode->id);
            commands.push_back("PUSH mem[" + to_string(loc) + "]");
        }

        else if (currentNode->type == "ADD" || currentNode->type == "SUB" ||
                 currentNode->type == "MUL" || currentNode->type == "DIV")
        {
            nodeStack.push_back(currentNode->right); // Push right subtree first (LIFO)
            nodeStack.push_back(currentNode->left);  // Push left subtree second (LIFO)
            commands.push_back(currentNode->type);
        }

        else if (currentNode->type == ":=" && currentNode->left->type == "VAR")
        {
            string key = currentNode->left->id;
            int loc;
            int init_address = targ.symtable->search(key);
            // The case when variable is already present:
            if (init_address > -1)
            {
                loc = init_address;
            }
            // New variable
            else
            {
                loc = least_mem_loc.get_min();
                // cout << loc << "  out"
                //      << " " << key << endl;
                targ.symtable->assign_address(key, loc);
                least_mem_loc.pop();
            }
            nodeStack.push_back(currentNode->right);
            commands.push_back("mem[" + to_string(loc) + "]" + " = POP");
        }

        else if (currentNode->type == ":=" && currentNode->left->type == "DEL")
        {
            // Variable has already been removed from symbol table
            string k = currentNode->right->id;
            least_mem_loc.push_heap(targ.last_deleted);
            // cout << targ.last_deleted << "  in"
            //      << " " << k << endl;
            commands.push_back("DEL = mem[" + std::to_string(targ.last_deleted) + "]");
        }

        else if (currentNode->type == ":=" && currentNode->left->type == "RET")
        {
            nodeStack.push_back(currentNode->right);
            commands.push_back("RET = POP");
        }
    }
    nodeStack.clear();
    reverse(commands);
    return commands;
}

void EPPCompiler::write_to_file(vector<string> commands)
{
    ofstream outfile;
    outfile.open(output_file, ios::app);
    for (auto x : commands)
    {
        outfile << x << endl;
    }
}

EPPCompiler::~EPPCompiler()
{
    memory_size = 0;
    // ofstream of;
    //  of.open(output_file);
    //  of.ios::clear();
    //  of.close();
}

// int main()
// {
//     string output_file = "tom.txt";
//     EPPCompiler x(output_file, 5);
//     vector<vector<string>> code = {{"a",":=","-3"},
//                                     {"b",":=","3"},
//                                     {"c",":=","3"},
//                                     {"del",":=","b"},
//                                     {"del",":=","c"}};
//     x.compile(code);
// }
