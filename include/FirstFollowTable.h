#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

class FirstFollowTable
{
private:
    // 文法文件名称
    std::string __filename;
    // 文法左侧所有非终结符集合，使用vector使得内容按原始文法顺序存储，便于后续调试输出
    std::vector<std::string> __left;

    bool isTerminalChar(const std::string &);
    void calFirst();
    void calFollow();
    void calStrFirst();
    void calStrFollow();

public:
    // 文法行数
    int __lines;
    // 非终结符集，终结符集
    std::set<std::string> Vns, Vts;
    // first集
    std::map<std::string, std::set<std::string>> first;
    // follow集
    std::map<std::string, std::set<std::string>> follow;
    // 文法
    std::pair<std::string, std::vector<std::string>> *formula;

    FirstFollowTable(std::string);
    void initFormula();
    void calAll();
    void showAll();
    static std::string getFormulaStr(std::vector<std::string> &);
    template <typename T>
    static std::string genFormatStr(std::string, std::string, std::string, T);
    void displayFirst();
    void displayFollow();
};