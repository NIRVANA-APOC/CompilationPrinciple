#include "FirstFollowTable.h"
#include "Logging.h"

FirstFollowTable::FirstFollowTable(std::string __filename)
{
    this->__filename = __filename;
    std::fstream fin(this->__filename);
    std::string tmp;
    this->__lines = 0;
    while (getline(fin, tmp))
    {
        this->__lines++;
    }
    this->Vts.insert("#");
    this->formula = new std::pair<std::string, std::vector<std::string>>[this->__lines];
    this->__lines = 0;
}

// 判断该符号是否为终结符
bool FirstFollowTable::isTerminalChar(const std::string &__s)
{
    return (this->Vts.count(__s) == 1);
}

// 打印一个set的所有内容
void display(std::set<std::string> &__s)
{
    for (auto it = __s.begin(); it != __s.end(); ++it)
    {
        std::cout << *it;
    }
    std::cout << std::endl;
}

// 处理文法文件
void FirstFollowTable::initFormula()
{
    std::string str;
    std::ifstream fin(this->__filename);
    if (!fin.is_open())
    {
        Log(ERR, "file \"%s\" not found.", __filename.data());
        return;
    }
    // __all_symbol集合存放所有终结符和非终结符,便于后续计算终结符集
    std::set<std::string> __all_symbol;
    for (; getline(fin, str); this->__lines++)
    {
        int mid_pos = str.find("->");
        if (mid_pos == -1)
        {
            Log(ERR, "表达式格式有误, 以忽略");
            continue;
        }
        int sta_pos = 0, end_pos = mid_pos;
        while (str.at(sta_pos) == ' ')
        {
            sta_pos++;
        }
        for (int i = sta_pos; i < mid_pos; i++)
        {
            if (str.at(i) == ' ')
            {
                end_pos = i;
                break;
            }
        }
        this->formula[__lines].first = str.substr(sta_pos, end_pos - sta_pos);
        this->Vns.insert(formula[__lines].first);
        this->__left.push_back(formula[__lines].first);

        sta_pos = mid_pos + 2;
        // 由于文法文件没有最终的EOF符号,因此手动添加换行符以保证后续处理不会丢失文法最后一行
        str.append("\n");
        for (int i = sta_pos; i < (int)str.length(); i++)
        {
            if (str.at(i) == ' ' || str.at(i) == '\r' || str.at(i) == '\n')
            {
                end_pos = i;
                if (end_pos != sta_pos)
                {
                    std::string ns = str.substr(sta_pos + 1, end_pos - sta_pos - 1);
                    sta_pos = end_pos;
                    if (ns == std::string(""))
                    {
                        // 由于sta_pos 和 end_pos的判断问题,有时可能会引入空字符串,此时要进行排除
                        continue;
                    }
                    this->formula[__lines].second.push_back(ns);
                    __all_symbol.insert(ns);
                }
            }
        }
    }
    __left.erase(std::unique(__left.begin(), __left.end()), __left.end());
    // 通过做差集的方式得到终结符集
    std::set_difference(__all_symbol.begin(), __all_symbol.end(), Vns.begin(), Vns.end(), std::inserter(Vts, Vts.begin()));
    fin.close();
}

// 计算FIRST集
void FirstFollowTable::calFirst()
{
    int __pre = -1, __now = 0;
    while (__pre != __now)
    {
        __pre = __now;
        for (int i = 0; i < __lines; i++)
        {
            std::string str = formula[i].first;
            std::vector<std::string> element = formula[i].second;
            if (isTerminalChar(element[0]))
            {
                first[str].insert(element[0]);
            }
            else
            {
                for (int j = 0; j < (int)element.size(); j++)
                {
                    if (isTerminalChar(element[j]))
                    {
                        break;
                    }
                    for (auto t : first[element[j]])
                    {
                        if (t != "$")
                        {
                            first[str].insert(t);
                        }
                        else if (j == (int)element.size() - 1)
                        {
                            first[str].insert(t);
                        }
                    }
                    if (first[element[j]].find("$") == first[element[j]].end())
                    {
                        break;
                    }
                }
            }
            __now = 0;
            for (auto t : Vns)
            {
                __now += (int)first[t].size();
            }
        }
    }
}

// 计算FOLLOW集
void FirstFollowTable::calFollow()
{
    follow[*__left.begin()].insert("#");
    int __pre = -1, __now = 0;
    while (__pre != __now)
    {
        __pre = __now;
        for (int i = 0; i < __lines; i++)
        {
            std::string str = formula[i].first;
            std::vector<std::string> element = formula[i].second;
            for (int j = 0; j < (int)element.size() - 1; j++)
            {
                if (!isTerminalChar(element[j]))
                {
                    if (!isTerminalChar(element[j + 1]))
                    {
                        follow[element[j]].insert(first[element[j + 1]].begin(), first[element[j + 1]].end());
                        follow[element[j]].erase("$");
                    }
                    else
                    {
                        follow[element[j]].insert(element[j + 1]);
                    }
                }
            }
            for (int j = (int)element.size() - 1; j >= 0; j--)
            {
                if (!isTerminalChar(element[j]))
                {
                    follow[element[j]].insert(follow[str].begin(), follow[str].end());
                }
                else
                {
                    break;
                }
                if (first[element[j]].find("$") == first[element[j]].end())
                {
                    break;
                }
            }
        }
        __now = 0;
        for (auto n : Vns)
        {
            __now += (int)follow[n].size();
        }
    }
}

// 计算串的FIRST集
void FirstFollowTable::calStrFirst()
{

    for (int i = 0; i < __lines; i++)
    {
        std::string form = getFormulaStr(formula[i].second);
        __left.push_back(form);
        Log(DEBUG, "form: %s", form.data());
        for (auto s : formula[i].second)
        {
            if (first.count(s))
            {
                first[form].insert(first[s].begin(), first[s].end());
            }
            else
            {
                first[form].insert(s);
            }
            if (first[form].find("$") != first[form].end())
            {
                // 如果存在空字符
                first[form].erase("$");
            }
            else
                break;
        }
    }
}

//
void FirstFollowTable::calStrFollow()
{
}

// 计算所有first和follow集
void FirstFollowTable::calAll()
{
    calFirst();
    calFollow();
    calStrFirst();
}
// 打印first集
void FirstFollowTable::displayFirst()
{
    std::string __out;
    Log(INFO, "FIRST集: ");
    for (auto n : __left)
    {
        __out.clear();
        __out += "FIRST(" + n + "): { ";
        for (auto t : first[n])
        {
            __out += t + ", ";
        }
        __out += " }";
        Log(INFO, "%s", __out.data());
    }
}
// 打印follow集
void FirstFollowTable::displayFollow()
{
    std::string __out;
    Log(INFO, "FOLLOW集: ");
    for (auto n : __left)
    {
        __out.clear();
        __out += "FOLLOW(" + n + "): { ";
        for (auto t : follow[n])
        {
            __out += t + ", ";
        }
        __out += " }";
        Log(INFO, "%s", __out.data());
    }
}
// 打印文法,终结符集和非终结符集
void FirstFollowTable::showAll()
{
    std::string __out;

    Log(INFO, "输入文法: ");
    for (int i = 0; i < __lines; i++)
    {
        __out.clear();
        __out += formula[i].first + " -> ";
        for (auto s : formula[i].second)
        {
            __out += s;
        }
        Log(INFO, "%s", __out.data());
    }

    Log(INFO, "终结符集: ");
    __out.clear();
    for (auto t : Vts)
    {
        __out += t + " ";
    }
    Log(INFO, "%s", __out.data());

    Log(INFO, "非终结符集: ");
    __out.clear();
    for (auto n : Vns)
    {
        __out += n + " ";
    }
    Log(INFO, "%s", __out.data());
}
// 通过拼接字符串向量,得到文法右侧
// @return 对应文法右侧结果,也可以用作字符串拼接函数,每两个元素中间以空格隔开
std::string FirstFollowTable::getFormulaStr(std::vector<std::string> &__v)
{
    std::string __ret;
    for (auto f : __v)
    {
        __ret += f + " ";
    }
    if (!__ret.empty())
    {
        __ret.pop_back();
    }
    return __ret;
}

// 本来是想用作格式化输出两边有包围符号的字符串使用的,不过对整体工作用处不大,因此未投入使用
template <typename T>
std::string FirstFollowTable::genFormatStr(std::string __left, std::string __right, std::string __sep, T __t)
{
    std::string __ret;
    for (auto __c : __t)
    {
        __ret += __c + __sep;
    }
    if (!__ret.empty())
    {
        __ret.pop_back();
    }
    return __left + __ret + __right;
}