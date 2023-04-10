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
    int _index;
    std::string _filename;
    std::vector<std::string> _left;

    bool isTerminalChar(const std::string &);
    void calFirst();
    void calFollow();


public:
    std::set<std::string> Vns, Vts;
    std::map<std::string, std::set<std::string>> first;
    std::map<std::string, std::set<std::string>> follow;
    std::pair<std::string, std::vector<std::string>> *formula;

    FirstFollowTable(std::string);
    void readExps();
    void calAll();
    void showAll();
    void displayFirst();
    void displayFollow();
};