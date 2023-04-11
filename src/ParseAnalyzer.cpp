#include "ParseAnalyzer.h"

ParseAnalyzer::ParseAnalyzer(FILE *__output, std::string __input, std::string __grammar)
{
    this->__output = __output;
    this->__fft = new FirstFollowTable(__grammar);
    __fft->readExps();
    __fft->calAll();

    this->__lex = new LexicalAnalyzer;
    __lex->process(__lex->readfile(__input));
}

void ParseAnalyzer::showAll()
{
    __fft->showAll();
    __fft->displayFirst();
    __fft->displayFollow();
}

void ParseAnalyzer::initAnalyzerTable()
{
    for (int i = 0; i < __fft->__index; i++)
    {
        std::string &__lf = __fft->formula[i].first;
        std::vector<std::string> &__rg = __fft->formula[i].second;
        if (__lf == "#")
        {
            continue;
        }
        std::string form = FirstFollowTable::getFormulaStr(__rg);

        if (form != "$")
        {
            for (auto ts : __fft->first[form])
            {
                if (ts != "$")
                {
                    analyzer_table[__lf][ts].assign(__rg.rbegin(), __rg.rend());
                }
            }
        }
    }
    for (auto ns : __fft->Vns)
    {
        for (auto ts : __fft->follow[ns])
        {
            analyzer_table[ns][ts].push_back("$");
        }
    }
    initLiteralTable();
}

void ParseAnalyzer::initLiteralTable()
{
    std::string __out;
    for (auto ns : __fft->Vns)
    {
        for (auto ts : __fft->Vts)
        {
            __out.clear();
            __out += ns + " -> ";
            for (auto f : analyzer_table[ns][ts])
            {
                if (ns == "#" || ts == "$" || f == "")
                {
                    continue;
                }
                __out += f + ", ";
            }
            if (__out == ns + " -> ")
            {
                continue;
            }
            literal_table[ns][ts] = "(" + ns + ", " + ts + ") => [ " + __out + " ]";
        }
    }
}

void ParseAnalyzer::output(std::string filename)
{
    FILE *fout = fopen(filename.data(), "w");
    for (auto ns : __fft->Vns)
    {
        for (auto ts : __fft->Vts)
        {
            auto &__l = literal_table[ns][ts];
            if (__l == "")
            {
                continue;
            }
            fprintf(fout, "%s\n", __l.data());
            Log(INFO, "%s", __l.data());
        }
    }
    fflush(fout);
}

void ParseAnalyzer::parse()
{
    std::stack<std::string> symbol;
    std::stack<std::string> input;
    bool __flag = false;

    symbol.push("#");
    input.push("#");
    symbol.push(__fft->formula[0].first);

    std::reverse(__lex->result.begin(), __lex->result.end());

    for (auto i : __lex->result)
    {
        if (i.first >= 100 || i.first < 0)
        {
            continue;
        }
        input.push(inputHandle(i));
    }

    int cnt = 0;
    while (!input.empty() && !symbol.empty())
    {
        auto &sym = symbol.top();
        auto &str = input.top();
        if (checkIDN(symbol.top()) == checkIDN(input.top()))
        {
            if (checkIDN(symbol.top()) == "EOF")
            {
                formatDisplay(cnt, symbol, input, "accept");
                __flag = true;
                break;
            }
            else
            {
                formatDisplay(cnt, symbol, input, "move");
                if (symbol.empty() || input.empty())
                {
                    break;
                }
                symbol.pop();
                input.pop();
            }
        }
        else
        {
            formatDisplay(cnt, symbol, input, "reduction");
            // std::cout << literal_table[sym][str] << std::endl;
            if (symbol.empty())
            {
                break;
            }
            symbol.pop();
            if (!analyzer_table[sym].count(str))
            {
                break;
            }
            for (auto n : analyzer_table[sym][str])
            {
                if (n == "$")
                {
                    continue;
                }
                symbol.push(checkIDN(n));
            }
        }
    }
    if (!__flag)
    {
        formatDisplay(cnt, symbol, input, "error");
    }

    return;
}

std::string ParseAnalyzer::checkIDN(const std::string &__in)
{
    if (__in == "Ident")
    {
        return "IDN";
    }
    else if (__in == "#")
    {
        return "EOF";
    }
    else
        return __in;
}

std::string ParseAnalyzer::inputHandle(const T &__t)
{
    if (__t.first == getSymCode("IDN"))
    {
        return "Ident";
    }
    else if (__t.first == getSymCode("INT"))
    {
        return "INT";
    }
    else
    {
        return __t.second;
    }
}

void ParseAnalyzer::formatDisplay(int &cnt, const std::stack<std::string> &symbol, const std::stack<std::string> &input, std::string msg)
{
    ++cnt;
    Log(INFO, "%d\t%s#%s\t%s", cnt, checkIDN(symbol.top()).data(), checkIDN(input.top()).data(), msg.data());
    if (__output)
    {
        fprintf(__output, "%d\t%s#%s\t%s\n", cnt, checkIDN(symbol.top()).data(), checkIDN(input.top()).data(), msg.data());
    }
}

void ParseAnalyzer::start()
{
    this->initAnalyzerTable();
    this->parse();
}