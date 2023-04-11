#pragma once

#include "Logging.h"
#include "ParseAnalyzer.h"

class Test
{
private:
    int id;
    std::string testfile;
    bool flag = false;

public:
    Test(int, std::string);
    void start();
};
