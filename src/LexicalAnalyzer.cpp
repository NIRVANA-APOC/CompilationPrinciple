#include <LexicalAnalyzer.h>
#include <Logging.h>


// 从符号表中查询对应的符号码,默认返回IDN码
int getSymCode(std::string sym)
{
    auto res = symbol_table.find(sym);
    return (res == symbol_table.end()) ? (symbol_table.find("UNKNOWN")->second) : res->second;
}

// 从符号表中查询对应的符号码,默认返回_default码
int getSymCode(std::string sym, std::string _default)
{
    auto res = symbol_table.find(sym);
    return (res == symbol_table.end()) ? (symbol_table.find(_default)->second) : res->second;
}

// 打印符号表
void showAll()
{
    Log(DEBUG, "Symbol Table: ");
    for (const auto &[s, i] : symbol_table)
    {
        Log(INFO, "%s\t\t%2d", s.data(), i);
    }
}

// 读取文件，以std::string返回文件内容
std::string LexicalAnalyzer::readfile(std::string filename)
{
    std::ifstream fin(filename.data());
    if (!fin.is_open())
    {
        Log(ERR, "file \"%s\" not found.", filename.data());
        return nullptr;
    }

    fin.seekg(0, std::ios::end);
    int size = fin.tellg();
    fin.seekg(0, std::ios::beg);

    char *buf = new char[size];
    fin.read(buf, size);
    fin.close();
    return std::string(buf);
}

// 判断当前字符是否为字母
bool LexicalAnalyzer::isLetter(char ch)
{
    return (
        (ch >= 'a' && ch <= 'z') ||
        (ch >= 'A' && ch <= 'Z'));
}

// 判断当前字符是否为数字
bool LexicalAnalyzer::isDigit(char ch)
{
    return (ch >= '0' && ch <= '9');
}

// 将当前字符转换成对应整型数字
int LexicalAnalyzer::toDigit(char ch)
{
    return (ch - '0');
}

/*
* 对当前字符串进行扫描
* 每次扫描得到一个符号表中对应的符号，并以(syn, token)的形式存入result内
*/ 
void LexicalAnalyzer::scan(std::string s)
{
    if (s.at(this->index) == ' ')
    {
        this->syn = getSymCode("SP");
        this->index++;
    }
    else
    {
        this->token.clear();
        auto ch = s.at(this->index);

        // 1.判断字符是否为数字 -*[0-9]+
        if (isDigit(ch) || ch == '-')
        {
            if(ch == '-'){
                this->token += '-';
                this->index++;
            }
            for (; isDigit(s.at(this->index)); this->index++)
            {
                this->token += s.at(this->index);
            }
            this->syn = getSymCode(this->token, "INT");
        }
        // 2.判断是否为标识符 [a-zA-Z_][a-zA-Z_0-9]* 或 关键字
        else if (isLetter(ch) || ch == '_')
        {
            for (
                auto ch = s.at(this->index);
                isLetter(ch) || isDigit(ch) || ch == '_';
                this->index++, ch = s.at(this->index))
            {
                this->token += s.at(this->index);
            }
            this->syn = getSymCode(this->token, "IDN");
        }
        // 3.其余均按照运算符处理
        else
        {
            this->token += s.at(this->index);
            this->index++;
            this->syn = getSymCode(this->token);

            switch (this->token[0])
            {
            case '=':
            case '<':
            case '>':
            case '!':
                if (s.at(this->index) == '=')
                {
                    this->token += s.at(this->index);
                    this->index++;
                    this->syn = getSymCode(this->token);
                }
                break;
            case '&':
            case '|':
                if (s.at(this->index) == this->token[0])
                {
                    this->token += s.at(this->index);
                    this->index++;
                    this->syn = getSymCode(this->token);
                }
                break;
            case '#':
                this->syn = getSymCode("END");
                break;
            case '\n':
                this->token = "\\n";
                this->syn = getSymCode("LF");
                break;
            case '\r':
                this->token = "\\r";
                this->syn = getSymCode("CR");
                break;
            case '\t':
                this->token = "\\t";
                this->syn = getSymCode("TAB");
                break;
            }
        }
    }
}

// 循环使用scan函数处理读入的字符串，直到遇到#为止
LexicalAnalyzer *LexicalAnalyzer::process(std::string s)
{
    s.push_back('#');
    do
    {
        LexicalAnalyzer::scan(s);
        switch (this->syn)
        {
        case -1:
            break;
        case -2:
            Log(ERR, "ERR");
            break;
        case -3:
            Log(INFO, "END");
            break;
        default:
            this->result.push_back(T(this->syn, this->token));
        }
    } while (this->syn != -3);
    return this;
}

// 以指定格式打印result内的结果
void LexicalAnalyzer::display()
{
    Log(DEBUG, "Display:");
    FILE* fp = fopen("Lex.txt", "w");
    for (const auto &p : this->result)
    {
        std::string symbol = getKeyWord(p.first);
        Log(INFO, "( %2d, %s )\t<%s>", p.first, p.second.data(), symbol.data());
        if(symbol != this->keyWord[5]){
            fprintf(fp, "%s\t<%s>\n", p.second.data(), symbol.data());
        }
    }
}

// 获取syn码对应的符号类型名称
std::string LexicalAnalyzer::getKeyWord(int syn){
    if(syn < 10){
        return this->keyWord[0];
    }
    else if(syn < 30){
        return this->keyWord[1];
    }
    else if(syn < 40){
        return this->keyWord[2];
    }
    else if(syn < 50){
        return this->keyWord[3];
    }
    else if(syn < 60){
        return this->keyWord[4];
    }
    else return this->keyWord[5];
}