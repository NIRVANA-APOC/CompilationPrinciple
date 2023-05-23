#include "DFA.h"


const closure_type DFA::move(const closure_type& S, const std::string& sym) const{
    closure_type ans;
    for(const auto &s: S){
        ans.insert(this->nfa->matrix[s][sym]);
    }
    return ans;
}

void DFA::minimizeDFA(){
    
}