#include <iostream>
#include "LexicalAnalyzer.h"
#include "FirstFollowTable.h"
#include "ParseAnalyzer.h"
#include "Config.h"
#include "Logging.h"
#include "Test.h"

FILE *log_fp;
// Config cfg;

int main(int argc, char **argv)
{
    LOG_ON("../run.log")

    std::string __input("../test/00/00.txt"), __grammar("../grammar.txt");

    if(argc == 2){
        __input = argv[1];
    }
    else if(argc == 3){
        __input = argv[1];
        __grammar = argv[2];
    }

    // ParseAnalyzer __parse(nullptr, __input, __grammar);

    Test t0(0, "../test/00/00.txt");
    // Test t1(1, "../test/01/01.txt");
    // Test t2(2, "../test/02/02.txt");
    // Test t7(7, "../test/07/07.txt");
    // Test t8(8, "../test/08_编译错误示例/08.txt");
    // Test t10(10, "../test/10_编译错误示例/10.txt");

    LOG_OFF
}