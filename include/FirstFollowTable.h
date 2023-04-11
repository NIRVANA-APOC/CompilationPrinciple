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
    std::string __filename;
    std::vector<std::string> __left;

    bool isTerminalChar(const std::string &);
    void calFirst();
    void calFollow();
    void calStrFirst();
    void calStrFollow();

public:
    int __index;
    std::set<std::string> Vns, Vts;
    std::map<std::string, std::set<std::string>> first;
    std::map<std::string, std::set<std::string>> follow;
    std::pair<std::string, std::vector<std::string>> *formula;

    FirstFollowTable(std::string);
    void readExps();
    void calAll();
    void showAll();
    static std::string getFormulaStr(std::vector<std::string>&);
    template<typename T>
    static std::string genFormatStr(std::string, std::string, std::string, T);
    void displayFirst();
    void displayFollow();
};