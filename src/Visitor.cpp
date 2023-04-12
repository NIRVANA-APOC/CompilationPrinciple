#include "Visitor.h"

void Visitor::visitChildren(BasicBlock* bb){

}

void VarDef::Init(BasicBlock* bb){
    Visitor::visitChildren(bb);
    std::map<std::string, Value*> VariableCollection = s;
    if(VariableCollection.find(bb->get_name()) != VariableCollection.end()){
        std::cout << "重複命名" << std::endl;
    }
    if(bb.is)
}