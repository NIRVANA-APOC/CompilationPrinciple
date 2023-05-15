#pragma once

#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include "Logging.h"

namespace fft
{
    using T = std::pair<std::string, std::vector<std::string>>;
}

class FirstFollowTable
{
private:
    std::string grammar_filepath;
    std::string output_filepath;
    bool _runover = false;

    void read();
    static std::string trim(std::string s);
    void formulaDisplay();
    void vnsDisplay();
    void vtsDisplay();

    bool isTerminalChar(const std::string &s);
    void calFirst();
    void calFollow();
    void calStrFirst();

public:
    std::set<std::string> Vns;
    std::set<std::string> Vts;
    std::map<std::string, std::set<std::string>> first;
    std::map<std::string, std::set<std::string>> follow;
    std::vector<fft::T> formula;
    std::vector<std::string> left_part;

    explicit FirstFollowTable(const std::string grammar_filepath, const std::string output_filepath = std::string("fft.txt")) : grammar_filepath(grammar_filepath), output_filepath(output_filepath) {}
    ~FirstFollowTable() = default;
    FirstFollowTable *run();
    FirstFollowTable *output();
    static std::string getFormulaStr(const std::vector<std::string> &v);
    bool has_run() { return this->_runover; }
};