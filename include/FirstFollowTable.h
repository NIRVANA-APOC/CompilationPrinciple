#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <regex>

class FirstFollowTable
{
private:
    std::string filename;
    std::pair<std::string, std::vector<std::string>> *exps;
    int index;
    std::set<std::string> Vns, Vts;
    std::vector<std::string> _left;
    std::map<std::string, std::set<std::string>> first;
    std::map<std::string, std::set<std::string>> follow;

public:
    FirstFollowTable(std::string);
    bool isTerminalChar(const std::string &);
    void readExps();
    void showAll();
    void calFirst();
    void calFollow();
    void calAll();
    void displayFirst();
    void displayFollow();
};