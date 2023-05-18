#pragma once
#include <string>
#include <vector>
#include <stack>
#include <map>

class AST_Node
{
private:
    int id;

    static int genId()
    {
        static int id = 0;
        return id++;
    }

public:
    std::vector<AST_Node*> childs;

    explicit AST_Node() : id(genId()) {}
    ~AST_Node() = default;
    AST_Node* addChild(AST_Node* child){
        this->childs.push_back(child);
        return this;
    }
};

class AST
{
private:
    std::vector<std::string> regexs;
    AST_Node S;

    void genAST(AST_Node* ast, std::string rgx);
    bool isOperator(char ch){
        return ((ch == '|') || (ch == '*'));
    }
public:
    AST* run();
    AST* output();
};