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
    closure_type states;
    closure_type syms;
    std::map<std::string, std::map<std::string, closure_type>> nfa;
    std::map<int, std::map<std::string, int>> matrix;

    void read();
    


public:
    explicit NFA(std::string input_filepath): input_filepath(input_filepath) {}
    NFA* run();
    NFA* output();
    const closure_type getSingleClosure(const closure_type& S, const std::string& sym);
    const closure_type getEpsilonClosure(const closure_type& S);
    const closure_type Ix(const closure_type& I, std::string x);
    void genMatrix(const closure_type& S);
};