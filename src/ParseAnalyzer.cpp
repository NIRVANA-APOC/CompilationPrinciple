#include "ParseAnalyzer.h"

void ParseAnalyzer::initAnalyzerTable()
{
    for (const auto &[l, r] : this->fft->formula)
    {
        if (l == "#")
        {
            continue;
        }
        auto form = FirstFollowTable::getFormulaStr(r);

        if (form != "$")
        {
            for (const auto &ts : this->fft->first[form])
            {
                if (ts != "$")
                {
                    analyzer_table[l][ts].assign(r.rbegin(), r.rend());
                }
            }
        }
    }

    for (const auto &ns : this->fft->Vns)
    {
        for (const auto &ts : this->fft->follow[ns])
        {
            analyzer_table[ns][ts].push_back("$");
        }
    }
}

ParseAnalyzer *ParseAnalyzer::run()
{
    using par::Status;
    if (!lex->has_run())
        lex->run();
    if (!fft->has_run())
        fft->run();
    this->initAnalyzerTable();
    using par::inputHandle, par::checkStr;
    std::stack<std::string> symbol;
    std::stack<std::string> input;
    static bool _runover = false;
    if (_runover)
        return this;

    symbol.push("#");
    input.push("#");
    symbol.push(this->fft->formula.front().first);

    std::reverse(this->lex->result.begin(), this->lex->result.end());
    for (const auto &l : this->lex->result)
    {
        if (l.first >= 100 || l.first < 0)
            continue;
        input.push(inputHandle(l));
    }
    auto state = Status::START;
    auto sym = symbol.top();
    auto inp = input.top();
    while (state != Status::ERROR)
    {
        formatDisplay(sym, inp, state);
        sym = symbol.top();
        inp = input.top();
        symbol.pop();

        if (sym == "#")
        {
            if (sym == inp)
            {
                state = Status::ACCEPT;
                formatDisplay(sym, inp, state);
                break;
            }
            else{
                state = Status::ERROR;
                break;
            }
        }
        else if (fft->Vts.count(sym))
        {
            if (sym == inp)
            {
                state = Status::MOVE;
                input.pop();
            }
            else
            {
                state = Status::ERROR;
                break;
            }
        }
        else if (analyzer_table[sym].count(inp))
        {
            state = Status::REDUCTION;
            for (const auto &a : analyzer_table[sym][inp])
            {
                if (a == "$")
                    continue;
                symbol.push(a);
            }
        }
        else
            state = Status::REDUCTION;
    }

    if (state == Status::ERROR)
    {
        formatDisplay(symbol.top(), input.top(), Status::ERROR);
    }
    std::reverse(this->lex->result.begin(), this->lex->result.end());
    _runover = true;
    return this;
}

ParseAnalyzer *ParseAnalyzer::output()
{
    lex->output();
    fft->output();
    if (this->output_filepath.empty())
    {
        Log(INFO, "未指定输出文件路径");
        return this;
    }
    FILE *fout = fopen(this->output_filepath.data(), "w");
    if (!fout)
    {
        Log(INFO, "输出文件打开失败");
        return this;
    }
    for (const auto &o : this->_output_log)
    {
        fprintf(fout, "%s\n", o.data());
    }
    fclose(fout);
    return this;
}

std::string par::inputHandle(const par::T &t)
{
    using lex::sym2syn;
    if (t.first == sym2syn("IDN", "UNKNOWN"))
    {
        return std::string("IDN");
    }
    else if (t.first == sym2syn("INT", "UNKNOWN"))
    {
        return std::string("INT");
    }
    else
    {
        return t.second;
    }
}

std::string par::checkStr(const std::string &s)
{
    if (s == "#")
    {
        return "EOF";
    }
    else
    {
        return s;
    }
}

void ParseAnalyzer::formatDisplay(const std::string &sym, const std::string &inp, const par::Status &state)
{
    using par::checkStr, par::Status;
    static int cnt = 0;
    std::string msg;
    switch (state)
    {
    case Status::START:
        return;
    case Status::REDUCTION:
        msg = "reduction";
        break;
    case Status::MOVE:
        msg = "move";
        break;
    case Status::ACCEPT:
        msg = "accept";
        break;
    case Status::ERROR:
        msg = "error";
        break;
    }
    ++cnt;
    std::string buf = std::to_string(cnt) + "\t" + checkStr(sym) + "#" + checkStr(inp) + "\t" + msg;
    Log(INFO, "%s", buf.data());
    _output_log.push_back(buf);
}

void ParseAnalyzer::analyzertableDisplay()
{
    for (const auto &[l, r] : this->analyzer_table)
    {
        for (const auto &[ll, rr] : r)
        {
            std::cout << l << " " << ll << " " << convert(rr) << " " << std::endl;
        }
    }
}
