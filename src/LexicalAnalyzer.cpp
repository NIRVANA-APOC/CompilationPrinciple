#include "LexicalAnalyzer.h"

const int lex::sym2syn(std::string sym, std::string _default = "UNKNOWN")
{
    auto res = symbol_table.find(sym);
    return (res == symbol_table.end()) ? symbol_table.find(_default)->second : res->second;
}

const std::string &lex::syn2key(int syn)
{
    for (const auto &k : lex::keyword_table)
    {
        if (syn < k.first)
        {
            return k.second;
        }
    }
    return lex::keyword_table.back().second;
}

bool lex::isLetter(char ch)
{
    return (
        (ch >= 'a' && ch <= 'z') ||
        (ch >= 'A' && ch <= 'Z'));
}

bool lex ::isDigit(char ch)
{
    return (ch >= '0' && ch <= '9');
}

void LexicalAnalyzer::read()
{

    std::ifstream fin(this->input_filepath.data());
    if (!fin.is_open())
    {
        Log(ERR, "file \"%s\" not found.", input_filepath.data());
        this->file_content = std::string();
    }

    fin.seekg(0, std::ios::end);
    int size = fin.tellg();
    fin.seekg(0, std::ios::beg);

    char *buf = new char[size];
    fin.read(buf, size);
    fin.close();

    this->file_content = std::string(buf);
}

lex::T LexicalAnalyzer::scan()
{
    using lex::isDigit, lex::isLetter;
    static int index = 0;
    auto ch = [&]
    { return this->file_content.at(index); };
    struct
    {
        std::string token;
        int syn;
        void set(std::string token, std::string _default = "UNKNOWN")
        {
            this->token = token;
            syn = lex::sym2syn(this->token, _default);
            index++;
        }

        void append(char ch, std::string _default = "UNKNOWN")
        {
            this->token += ch;
            syn = lex::sym2syn(this->token, _default);
            index++;
        }
    } token;

    if (ch() == ' ')
    {
        token.set("SPACE");
    }
    else
    {
        token.token.clear();

        if (isDigit(ch()) || std::set<char>{'-', '+', '!'}.count(ch()))
        {
            if (!isDigit(ch()))
            {
                token.append(ch());
            }
            if (this->result.back().first != lex::sym2syn("INT"))
            {
                while (isDigit(ch()))
                {
                    token.append(ch(), "INT");
                }
            }
        }
        else if (isLetter(ch()) || ch() == '_')
        {
            while (isLetter(ch()) || isDigit(ch()) || ch() == '_')
            {
                token.append(ch(), "IDN");
            }
        }
        else
        {
            token.append(ch());

            switch (token.token.front())
            {
            case '=':
            case '<':
            case '>':
            case '!':
                if (ch() == '=')
                {
                    token.append('=');
                }
                break;
            case '&':
            case '|':
                if (ch() == token.token.front())
                {
                    token.append(ch());
                }
                break;
            case '\n':
                token.token = "LF";
                break;
            case '\r':
                token.token = "CR";
                break;
            case '\t':
                token.token = "TAB";
                break;
            case '#':
                token.token = "END";
                token.syn = lex::sym2syn("END");
                break;
            }
        }
    }
    return lex::T(token.syn, token.token);
}

LexicalAnalyzer *LexicalAnalyzer::run()
{
    if (_runover)
        return this;
    this->read();
    this->file_content.push_back('#');
    for (;;)
    {
        auto r = this->scan();
        switch (r.first)
        {
        case -2:
            Log(ERR, "ERR < %s >", r.second.data());
            break;
        case -3:
            Log(INFO, "END");
            _runover = true;
            return this;
        case -4:
            Log(INFO, "UNKONWN < %s >", r.second.data());
            break;
        default:
            this->result.push_back(r);
        }
    }
}

LexicalAnalyzer *LexicalAnalyzer::output()
{
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
    for (const auto &r : this->result)
    {
        auto key = lex::syn2key(r.first);
        if (key != "UKN")
        {
            fprintf(fout, "%s\t<%s>\n", r.second.data(), key.data());
        }
    }
    fclose(fout);
    return this;
}