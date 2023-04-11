#pragma once

#include "LexicalAnalyzer.h"
#include "FirstFollowTable.h"
#include "Logging.h"
#include <string>
#include <map>
#include <stack>
#include <algorithm>

using T = std::pair<int, std::string>;

const static std::map<std::string, std::string> convert_table = {
    {"IDN", "Ident"},
};

class ParseAnalyzer
{
    friend class FirstFollowTable;

private:
    FILE *__output;
    LexicalAnalyzer *__lex;
    FirstFollowTable *__fft;
    std::map<std::string, std::map<std::string, std::vector<std::string>>> analyzer_table;
    std::map<std::string, std::map<std::string, std::string>> literal_table;

public:
    ParseAnalyzer(FILE *, std::string, std::string);
    void showAll();
    void initAnalyzerTable();
    void literalTable();
    void output(std::string);
    void parse();
    void start();
    std::string checkIDN(const std::string &);
    std::string inputHandle(const T &);
    void formatDisplay(int &, const std::stack<std::string> &, const std::stack<std::string> &, std::string);
};