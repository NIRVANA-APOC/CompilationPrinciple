#include "ParseAnalyzer.h"

ParseAnalyzer::ParseAnalyzer(std::string filename){
    this->fft = new FirstFollowTable(filename);
    fft->readExps();
    fft->calAll();
}

void ParseAnalyzer::showAll(){
    fft->showAll();
    fft->displayFirst();
    fft->displayFollow();
}

void ParseAnalyzer::initAnalyzerTable(){
    for(auto ns: fft->Vns){
        for(auto ts: fft->first[ns]){
            
        }
    }
}