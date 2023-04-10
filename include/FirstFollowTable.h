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
    int index;
    std::string filename;
    std::pair<std::string, std::vector<std::string>> *formula;
    std::set<std::string> Vns, Vts;
    std::vector<std::string> _left;
    std::map<std::string, std::set<std::string>> first;
    std::map<std::string, std::set<std::string>> follow;

    bool isTerminalChar(const std::string &);
    void calFirst();
    void calFollow();

public:
    FirstFollowTable(std::string);
    void readExps();
    void calAll();
    void showAll();
    void displayFirst();
    void displayFollow();
};