#include <iostream>
#include "LexicalAnalyzer.h"
#include "FirstFollowTable.h"
#include "Logging.h"

FILE *log_fp;

int main(int argc, char **argv)
{
    std::string filename{"test.cmm"};
    if (argc == 2)
    {
        filename = argv[1];
    }
    log_fp = fopen("run.log", "w");
    LexicalAnalyzer lex;
    showAll();
    lex.process(lex.readfile(filename))->display();

    FirstFollowTable fft("grammar.txt");
    fft.readExps();
    fft.showAll();
    fft.calAll();
    fft.displayFirst();
    fft.displayFollow();
    fclose(log_fp);
}