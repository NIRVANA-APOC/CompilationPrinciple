#pragma once

#include "NFA.h"
#include <map>

class DFA{
private:
    NFA* nfa;

public:
    DFA(NFA* nfa):
        nfa(nfa){}
    DFA(std::string nfa_input_filepath):
        nfa(new NFA(nfa_input_filepath)) {}
    ~DFA() = default;
    const closure_type move(const closure_type& S, const std::string& sym) const;
    void minimizeDFA();
};