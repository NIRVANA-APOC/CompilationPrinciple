#pragma once
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <exception>

using T = std::pair<int, std::string>;

// 符号表
const static std::map<std::string, int> symbol_table{
    {"NS", -5},
    {"UNKNOWN", -4},
    {"END", -3},
    {"ERR", -2},
    // SPACE
    {"SP", -1},
    // KW
    {"int", 0},
    {"void", 1},
    {"return", 2},
    {"const", 3},
    // OP
    {"+", 10},
    {"-", 11},
    {"*", 12},
    {"/", 13},
    {"%", 14},
    {"=", 15},
    {">", 16},
    {"<", 17},
    {"==", 18},
    {"<=", 19},
    {">=", 20},
    {"!=", 21},
    {"&&", 22},
    {"||", 23},
    // SE
    {"(", 30},
    {")", 31},
    {"{", 32},
    {"}", 33},
    {";", 34},
    {",", 35},
    // IDN
    {"IDN", 40},
    // INT
    {"INT", 50},
    {"CR", 100},
    {"LF", 101},
    {"TAB", 102},
};

void showAll();
int getSymCode(std::string);
int getSymCode(std::string, std::string);

// 全局参数
class LexicalAnalyzer
{
private:
    // 符号种别
    const std::string keyWord[6]{
        "KW",
        "OP",
        "SE",
        "IDN",
        "INT",
        "UKN",
    };
    // 单词种别码
    int syn;
    // 单词字符串
    std::string token;
    // 当前扫描位置
    int __index = 0;

    bool isLetter(char);
    bool isDigit(char);
    int toDigit(char);
    void scan(std::string &);

public:
    std::vector<T> result;
    std::string readfile(std::string);
    LexicalAnalyzer *process(std::string);
    void display();
    std::string getKeyWord(int);
};
