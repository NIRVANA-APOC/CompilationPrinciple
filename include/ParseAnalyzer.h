#pragma once

#include "LexicalAnalyzer.h"
#include "FirstFollowTable.h"
#include "Logging.h"
#include <string>
#include <map>
#include <stack>
#include <algorithm>

namespace par
{
    using T = std::pair<int, std::string>;
    /*!
     *@brief  检查词是否为特殊终结符
     *@brief 由于原始设计时对词法分析的结果采用了<种别码, 词>的形式,因此在进行语法分析的时候,对于特殊的种别码,例如INT, IDN要单独进行转换后再插入
     *@return 经过处理后的字符串
     */
    std::string inputHandle(const par::T &t);
    /*!
     *@brief 检查字符串中是否含有特殊词语，主要是由于有些字符串例如"Ident",在输出时要输出成"IDN",而在内部语法分析时要以非终结符"Ident"存在,因此要做特别处理
     *@return 经过处理后的字符串
     */
    std::string checkStr(const std::string &s);
}

/*!
 *@brief 语法分析器
 *@param lex 内置词法分析器
 *@param fft 内置First集和Follow集分析器
 *@param output_filepath 输出文件的路径
 *@param analyzer_table 存储语法分析结果，以向量形式存储，便于后续压栈时使用
 *@param literal_table 存储语法分析结果，以字符串形式存储，便于调试输出时使用
 *@param _output_log 存储归约时的打印日志，以便后续output函数调用
 */
class ParseAnalyzer
{
private:
    LexicalAnalyzer *lex;
    FirstFollowTable *fft;
    std::string output_filepath;
    std::map<std::string, std::map<std::string, std::vector<std::string>>> analyzer_table;
    std::vector<std::string> _output_log;

    /*!
     *@brief 初始化语法分析表
     */
    void initAnalyzerTable();
    void formatDisplay(const std::string &sym, const std::string &inp, const std::string &msg);

public:
    /*!
     *@brief 构造函数
     *@param lex 词法分析器对象
     *@param fft 内置First集和Follow集分析器对象
     *@param output_filepath 输出文件的路径
     *@return 当前对象本身
     */
    explicit ParseAnalyzer(LexicalAnalyzer *lex = new LexicalAnalyzer("lexin.txt"), FirstFollowTable *fft = new FirstFollowTable("fftin.txt"), const std::string output_filepath = "par.txt") : lex(lex), fft(fft), output_filepath(output_filepath) {}
    ~ParseAnalyzer() = default;
    /*!
     *@brief 进行语法分析
     *@return 当前对象本身
     */
    ParseAnalyzer *run();
    /*!
     *@brief 将语法分析结果输出到指定文件中
     *@return 当前对象本身
     */
    ParseAnalyzer *output();
};