#include <iostream>
#include "LexicalAnalyzer.h"
#include "FirstFollowTable.h"
#include "ParseAnalyzer.h"
#include "FA.h"
#include "Logging.h"

FILE *log_fp;

int main()
{
    LOG_ON("../log.txt")

    // 输入NFA图并进行NFA确定化
    // FA *nfa = new FA("../nfa.txt");
    // nfa->read()->minimizeDFA(closure{"9", "3", "4", "5", "6"}, closure{"1", "2", "7", "8"});
    // nfa->output();
    // 定义词法分析器并输入文件
    LexicalAnalyzer *lex = new LexicalAnalyzer("../test.txt");
    lex->run()->output();
    // 定义FirstFollow集并输入语法文件
    FirstFollowTable *fft = new FirstFollowTable("../grammar.txt");
    fft->run()->output();
    fft->vtsDisplay();
    fft->vnsDisplay();
    // 定义语法分析器
    ParseAnalyzer *par = new ParseAnalyzer(lex, fft);
    par->run()->output();
    par->analyzertableDisplay();
    LOG_OFF
}