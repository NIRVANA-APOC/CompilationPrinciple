#include <iostream>
#include "LexicalAnalyzer.h"
#include "FirstFollowTable.h"
#include "ParseAnalyzer.h"
#include "NFA.h"
#include "DFA.h"
#include "Logging.h"

FILE* log_fp;

int main(){
    LOG_ON("../log.txt")

    // LexicalAnalyzer* lex = new LexicalAnalyzer("../test.txt");
    // // lex->run()->output();

    // FirstFollowTable* fft = new FirstFollowTable("../grammar.txt");
    // // fft->run()->output();

    // ParseAnalyzer* par = new ParseAnalyzer(lex, fft);
    // par->run()->output();
    NFA* nfa = new NFA("../nfa.txt");
    nfa->run();
    nfa->output();
    // auto &a = nfa->getNewPartition(nfa->states);
    // for(auto i: a){
    //     std::cout << convert(i) << std::endl;
    // }

    // std::cout << convert(nfa->move(nfa->states, "a").first) << std::endl;
    // std::cout << convert(nfa->move(nfa->states, "a").second) << std::endl;
    // std::cout << convert(nfa->move(nfa->states, "b").first) << std::endl;
    // std::cout << convert(nfa->move(nfa->states, "b").second) << std::endl;
    // std::cout << convert(nfa->move(closure_type{"0", "1", "2"}, "a").first) << std::endl;
    // std::cout << convert(nfa->move(closure_type{"0", "1", "2"}, "a").second) << std::endl;
    // std::cout << convert(nfa->move(closure_type{"0", "1", "2"}, "a").first) << std::endl;
    // std::cout << convert(nfa->move(closure_type{"0", "1", "2"}, "a").second) << std::endl;
    // std::cout << convert(nfa->move(closure_type{"3", "4", "5", "6"}, "a").first) << std::endl;
    // std::cout << convert(nfa->move(closure_type{"3", "4", "5", "6"}, "a").second) << std::endl;
    // std::cout << convert(nfa->move(closure_type{"3", "4", "5", "6"}, "b").first) << std::endl;
    // std::cout << convert(nfa->move(closure_type{"3", "4", "5", "6"}, "b").second) << std::endl;

    nfa->minimizeDFA(closure_type{"0", "1", "2"}, closure_type{"3", "4", "5", "6"});

    // DFA* dfa = new DFA(nfa);
    // closure_type a{"0", "1", "2"};
    // dfa->move(a, "a");
    
    LOG_OFF
}