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

template <typename T>
std::string convert(T t)
{
    std::string out;
    out += "{ ";
    for (const auto &_t : t)
    {
        out += _t + ", ";
    }
    out += " }";
    return out;
}

using closure = std::set<std::string>;
class FA
{
private:
    std::string input_filepath;
    std::string output_filepath;

    closure syms;
    std::map<std::string, std::map<std::string, closure>> nfa;
    std::map<std::string, std::map<std::string, std::string>> dfa;
    std::map<std::string, std::map<std::string, closure>> result;

    
    const closure getSingleClosure(const closure &S, const std::string &sym);
    const closure getEpsilonClosure(const closure &S);
    const closure Ix(const closure &I, std::string x);
    const std::pair<closure, closure> move(const closure &S, const std::string &sym);

public:
    // <a, b, c> -> from a via b to c
    std::map<std::string, std::map<std::string, std::string>> matrix;
    closure states;
    explicit FA(std::string input_filepath, std::string output_filepath = "FA.txt") : input_filepath(input_filepath), output_filepath(output_filepath) {}
    FA* determinNFA();
    FA* minimizeDFA(const closure &start, const closure &end);
    FA* read(bool is_dfa=false);
    FA *output();
};