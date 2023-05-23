#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <sstream>
#include <functional>
#include "Logging.h"

template<typename T>
std::string convert(T t){
    std::string out;
    out += "{ ";
    for(const auto &_t: t){
        out += _t + ", ";
    }
    out += " }";
    return out;
}

using closure_type = std::set<std::string>;
class NFA{
private:
    std::string input_filepath;
    
    closure_type syms;
    std::map<std::string, std::map<std::string, closure_type>> nfa;
    std::map<std::string, std::map<std::string, std::string>> dfa;

    void read();
    void determinNFA();
    

public:
    // <a, b, c> -> from a via b to c
    std::map<std::string, std::map<std::string, std::string>> matrix;
    closure_type states;
    explicit NFA(std::string input_filepath): input_filepath(input_filepath) {}
    const closure_type getSingleClosure(const closure_type& S, const std::string& sym);
    const closure_type getEpsilonClosure(const closure_type& S);
    const closure_type Ix(const closure_type& I, std::string x);
    const std::pair<closure_type, closure_type> move(const closure_type& S, const std::string& sym);
    void minimizeDFA(const closure_type& start, const closure_type& end);
    NFA* run();
    NFA* output();
};