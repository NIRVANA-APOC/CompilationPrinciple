#include <iostream>
#include "LexicalAnalyzer.h"
#include "FirstFollowTable.h"
#include "ParseAnalyzer.h"
#include "Logging.h"

FILE *log_fp;

int main(int argc, char **argv)
{
    LOG_ON("run.log")

    std::string filename{"test.cmm"};
    if (argc == 2)
    {
        filename = argv[1];
    }

    // LexicalAnalyzer lex;
    // showAll();
    // lex.process(lex.readfile(filename))->display();

    // FirstFollowTable fft("../grammar.txt");
    // fft.readExps();
    // fft.showAll();
    // fft.calAll();
    // fft.displayFirst();
    // fft.displayFollow();

    ParseAnalyzer par("../grammar.txt");
    par.showAll();


    LOG_OFF
}