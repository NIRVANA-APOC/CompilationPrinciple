#pragma once

#include "LexicalAnalyzer.h"
#include "FirstFollowTable.h"
#include "Logging.h"
#include <string>
#include <map>
#include <stack>
#include <algorithm>

namespace par
{
    using T = std::pair<int, std::string>;
    std::string inputHandle(const par::T &t);
    std::string checkStr(const std::string &s);
}

class ParseAnalyzer
{
private:
    LexicalAnalyzer *lex;
    FirstFollowTable *fft;
    std::string output_filepath;
    std::map<std::string, std::map<std::string, std::vector<std::string>>> analyzer_table;
    std::map<std::string, std::map<std::string, std::string>> literal_table;
    std::vector<std::string> _output_log;

    void initAnalyzerTable();
    void initLiteralTable();
    void formatDisplay(const std::string &sym, const std::string &inp, const std::string &msg);

public:
    explicit ParseAnalyzer(LexicalAnalyzer *lex = new LexicalAnalyzer("lexin.txt"), FirstFollowTable *fft = new FirstFollowTable("fftin.txt"), const std::string output_filepath = "par.txt") : lex(lex), fft(fft), output_filepath(output_filepath) {}
    ~ParseAnalyzer() = default;
    ParseAnalyzer *run();
    ParseAnalyzer *output();
};