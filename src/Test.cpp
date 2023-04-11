#include "Test.h"

Test::Test(int id, std::string filename)
{
    this->id = id;
    this->testfile = filename;
    start();
}

void Test::start()
{
    std::string __out_dir = "../output/";
    FILE *__parse_out = fopen((__out_dir + std::to_string(this->id) + "_result.txt").data(), "w");
    ParseAnalyzer par(__parse_out, this->testfile, "../grammar.txt");
    par.start();
    par.output(__out_dir + std::to_string(this->id) + ".txt");
    fclose(__parse_out);
}