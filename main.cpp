#include <iostream>
#include <LexicalAnalyzer.h>
#include <Logging.h>

FILE* log_fp;

int main(){
    log_fp = fopen("run.log", "w");
    Log(DEBUG, "this is a test");
    LexicalAnalyzer lex;
    showAll();
    lex.process(lex.readfile("test.cmm"))->display();
    fclose(log_fp);
}