#include <iostream>
#include "LexicalAnalyzer.h"
#include "FirstFollowTable.h"
#include "ParseAnalyzer.h"
#include "Logging.h"

FILE* log_fp;

int main(){
    LOG_ON("../log.txt")

    LexicalAnalyzer* lex = new LexicalAnalyzer("../test.txt");
    // lex->run()->output();

    FirstFollowTable* fft = new FirstFollowTable("../grammar.txt");
    // fft->run()->output();

    ParseAnalyzer* par = new ParseAnalyzer(lex, fft);
    par->run()->output();

    LOG_OFF
}