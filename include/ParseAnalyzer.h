#pragma once

#include "FirstFollowTable.h"


class ParseAnalyzer{
    private:
    FirstFollowTable *fft;
    public:
    ParseAnalyzer(std::string);
    void showAll();
};