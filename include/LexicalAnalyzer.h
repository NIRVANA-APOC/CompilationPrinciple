#pragma once

#include <string>
#include <map>
#include <array>
#include <list>
#include <set>
#include "Logging.h"

namespace lex
{
    /*!
     *@brief 词法解析器pair对，<token种别码，token字符串>
     */
    using T = std::pair<int, std::string>;

    /*!
     * @brief 符号表
     */
    const static std::map<std::string, int> symbol_table{
        {"UNKNOWN", -4},
        {"END", -3},
        {"ERR", -2},
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
        {"SPACE", 103},
    };

    /*!
     *@brief 符号转种别码函数
     *@param sym 待转换的符号
     *@param _default 未找到对应种别码时默认返回的符号种别码，default="UNKNOWN"
     *@return 转换后的符号种别码
     */
    const int sym2syn(std::string sym, std::string _default);

    const static std::array<T, 7> keyword_table{
        T(0, "UKN"),
        T(10, "KW"),
        T(30, "OP"),
        T(40, "SE"),
        T(50, "IDN"),
        T(60, "INT"),
        T(100, "UKN"),
    };

    /*!
     *@brief 种别码转关键字函数
     *@param syn 待转换的种别码
     *@return 转换后的关键字字符串
     */
    const std::string &syn2key(int syn);

    bool isLetter(char ch);
    bool isDigit(char ch);
}

/*!
 *@brief 词法分析器
 *@param output_filepath 输出文件的路径
 *@param result 词法分析结果
 */
class LexicalAnalyzer
{
private:
    std::string input_filepath;
    std::string output_filepath;
    std::string file_content;
    bool _runover = false;

    /*!
     *@brief 读取文件内容，并存储在file_content私有成员内
     */
    void read();

    /*!
     *@brief 扫描器函数
     *@brief 逐字符扫描file_content内容
     *@return 成功：以std::pair类型返回第一个成功处理的<种别码，字符串token> 失败：返回<-2, ERR>
     */
    lex::T scan();

public:
    std::list<lex::T> result;

    /*!
     *@brief 词法解析器的构造函数
     *@param input_filepath 输入文件的路径
     *@param output_filepath 输出文件的路径
     *@return 当前对象本身
     */
    explicit LexicalAnalyzer(const std::string input_filepath, const std::string output_filepath = std::string("lex.txt"))
        : input_filepath(input_filepath), output_filepath(output_filepath){};
    ~LexicalAnalyzer() = default;

    /*!
     *@brief 词法解析函数，通过循环调用scan函数对输入字符串进行词法解析
     *@return 当前对象本身
     */
    LexicalAnalyzer *run();

    /*!
     *@brief 将词法分析结果输出到指定文件中
     *@return 当前对象本身
     */
    LexicalAnalyzer *output();

    /*!
     *@brief 判断该词法分析器是否调用过run函数
     */
    bool has_run() { return this->_runover; }
};