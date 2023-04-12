#pragma once

#include "IRbuilder.h"
#include <map>
#include <string>

std::map<std::string, Value*> s;

class Visitor
{
private:
    IRBuilder irb;

public:
    virtual void Init(BasicBlock *);
    virtual void Visit(BasicBlock *);
    static void visitChildren(BasicBlock *);
};

class VarDef : public Visitor
{
public:
    void Init(BasicBlock *);
    void Visit(BasicBlock *);
};
