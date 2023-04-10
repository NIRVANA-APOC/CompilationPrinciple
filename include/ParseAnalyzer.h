#pragma once

#include "FirstFollowTable.h"
#include <string>
#include <map>


class ParseAnalyzer{
    friend class FirstFollowTable;
    private:
    FirstFollowTable *fft;
    std::map<std::string, std::map<std::string, std::string>> analyzer_table;
    
    public:
    ParseAnalyzer(std::string);
    void showAll();
    void initAnalyzerTable();
};