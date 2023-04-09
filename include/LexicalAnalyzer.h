#pragma once
#include <map>
#include <string>
#include <vector>
#include <fstream>


using T = std::pair<int, std::string>;

// 符号表
const static std::map<std::string, int> symbol_table{
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
    {"CR",  60},
    {"LF",  61},
    {"TAB",  62},
};

void showAll();
int getSymCode(std::string);


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
    int index = 0;
    // 判断tag
    int tag = 1;
    // 存储处理结果
    std::vector<T> result;

    bool isLetter(char);
    bool isDigit(char);
    int toDigit(char);
    void scan(std::string);
    std::string getKeyWord(int);
public:
    std::string readfile(std::string);
    LexicalAnalyzer* process(std::string);
    void display();
};
