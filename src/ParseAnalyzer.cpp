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
    if (!lex->has_run())
    {
        lex->run();
    }
    if (!fft->has_run())
    {
        fft->run();
    }
    this->initAnalyzerTable();
    using par::inputHandle, par::checkStr;
    std::stack<std::string> symbol;
    std::stack<std::string> input;
    auto empty = [&]
    { return symbol.empty() || input.empty(); };
    static bool _runover = false;
    if (_runover)
    {
        return this;
    }

    symbol.push("#");
    input.push("#");
    symbol.push(this->fft->formula.front().first);

    std::reverse(this->lex->result.begin(), this->lex->result.end());
    for (const auto &l : this->lex->result)
    {
        if (l.first >= 100 || l.first < 0)
        {
            continue;
        }
        input.push(inputHandle(l));
    }

    while (!empty())
    {
        auto sym = symbol.top();
        auto inp = input.top();

        if (checkStr(sym) == checkStr(inp))
        {
            if (checkStr(sym) == "EOF")
            {
                formatDisplay(sym, inp, "accept");
                _runover = true;
                break;
            }
            else
            {
                if (empty())
                {
                    break;
                }
                formatDisplay(sym, inp, "move");
                symbol.pop();
                input.pop();
            }
        }
        else if (checkStr(sym) == "EOF")
        {
            break;
        }
        else
        {
            formatDisplay(sym, inp, "reduction");
            if (symbol.empty())
            {
                break;
            }
            symbol.pop();
            if (!analyzer_table[sym].count(inp))
            {
                break;
            }
            for (const auto &n : analyzer_table[sym][inp])
            {
                if (n == "$")
                {
                    continue;
                }

                symbol.push(checkStr(n));
            }
        }
    }
    if (!_runover)
    {
        formatDisplay(symbol.top(), input.top(), "error");
    }
    std::reverse(this->lex->result.begin(), this->lex->result.end());
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
        return std::string("Ident");
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
    if (s == "Ident")
    {
        return "IDN";
    }
    else if (s == "#")
    {
        return "EOF";
    }
    else
    {
        return s;
    }
}

void ParseAnalyzer::formatDisplay(const std::string &sym, const std::string &inp, const std::string &msg)
{
    using par::checkStr;
    static int cnt = 0;
    ++cnt;
    std::string buf = std::to_string(cnt) + "\t" + checkStr(sym) + "#" + checkStr(inp) + "\t" + msg;
    Log(INFO, "%s", buf.data());
    _output_log.push_back(buf);
}