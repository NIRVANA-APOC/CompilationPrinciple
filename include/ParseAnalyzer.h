#pragma once

#include "LexicalAnalyzer.h"
#include "FirstFollowTable.h"
#include "Logging.h"
#include <string>
#include <map>
#include <stack>
#include <algorithm>

using T = std::pair<int, std::string>;

// const static std::map<std::string, std::string> convert_table = {
//     {"IDN", "Ident"},
// };

// 语法分析器
class ParseAnalyzer
{
private:
    // 分析结果输出流
    FILE *__output;
    // 词法分析器
    LexicalAnalyzer *__lex;
    // first & follow集
    FirstFollowTable *__fft;
    // 存储语法分析结果，以向量形式存储，便于后续压栈时使用
    std::map<std::string, std::map<std::string, std::vector<std::string>>> analyzer_table;
    // 存储语法分析结果，以字符串形式存储，便于调试输出时使用
    std::map<std::string, std::map<std::string, std::string>> literal_table;

public:
    ParseAnalyzer(FILE *, std::string, std::string);
    void showAll();
    void initAnalyzerTable();
    void initLiteralTable();
    void output(std::string);
    void parse();
    void start();
    std::string checkStr(const std::string &);
    std::string inputHandle(const T &);
    void formatDisplay(int &, const std::stack<std::string> &, const std::stack<std::string> &, std::string);
    void visitor(std::string);
};