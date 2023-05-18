#include "AST.h"
// 连接，联合，闭包
void AST::genAST(AST_Node* ast, std::string rgx){
    std::stack<std::string> nodes;
    std::stack<std::string> ops;

    for(const auto &token: rgx){
        
    }
}

AST* AST::run(){
    AST_Node* root = &this->S;
    for(const auto &rgx: this->regexs){
        root->addChild(new AST_Node);
    }
    for(auto &s: root->childs){

    }
    return this;
}