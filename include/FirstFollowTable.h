#pragma once

#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include "Logging.h"

namespace fft
{
    using T = std::pair<std::string, std::vector<std::string>>;
}

/*!
 *@brief First集和Follow集
 *@param grammar_filepath 输入的文法文件路径
 *@param output_filepath 输出文件的路径
 *@param Vns 非终结符集
 *@param Vts 终结符集
 *@param first first集
 *@param follow follow集
 *@param formula 文法
 *@param left_part 文法左侧所有非终结符集合，使用vector使得内容按原始文法顺序存储，便于后续调试输出
 */
class FirstFollowTable
{
private:
    std::string grammar_filepath;
    std::string output_filepath;
    bool _runover = false;

    /*!
     *@brief 读取文法文件内容，并将处理好的文法存储在formula里
     */
    void read();
    /*!
     *@brief 去除字符串首尾空格
     *@param s 目标字符串
     *@return 去除首尾空格后的字符串
     */
    static std::string trim(std::string s);

    bool isTerminalChar(const std::string &s);
    void calFirst();
    void calFollow();
    void calStrFirst();

public:
    std::set<std::string> Vns;
    std::set<std::string> Vts;
    std::map<std::string, std::set<std::string>> first;
    std::map<std::string, std::set<std::string>> follow;
    std::vector<fft::T> formula;
    std::vector<std::string> left_part;

    void formulaDisplay();
    void vnsDisplay();
    void vtsDisplay();

    /*!
     *@brief 构造函数
     *@param grammar_filepath 文法文件的路径
     *@param output_filepath 输出文件的路径
     *@return 当前对象本身
     */
    explicit FirstFollowTable(const std::string grammar_filepath, const std::string output_filepath = std::string("fft.txt")) : grammar_filepath(grammar_filepath), output_filepath(output_filepath) {}
    ~FirstFollowTable() = default;
    /*!
     *@brief 计算文法的First集和Follow集
     *@return 当前对象本身
     */
    FirstFollowTable *run();
    /*!
     *@brief 将文法分析结果输出到指定文件中
     *@return 当前对象本身
     */
    FirstFollowTable *output();
    /*!
     *@brief 将向量形式的字符串以空格为分隔符进行拼接
     *@return 拼接后的字符串
     */
    static std::string getFormulaStr(const std::vector<std::string> &v);
    /*!
     *@brief 判断该词法分析器是否调用过run函数
     */
    bool has_run() { return this->_runover; }
};