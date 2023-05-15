#include "FirstFollowTable.h"

bool FirstFollowTable::isTerminalChar(const std::string &s)
{
    return static_cast<bool>(this->Vts.count(s));
}

std::string FirstFollowTable::trim(std::string s)
{
    if (!s.empty())
    {
        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
    }
    return s;
}

void FirstFollowTable::read()
{
    std::string buf;
    std::ifstream fin(this->grammar_filepath);
    int lines = 0;
    if (!fin.is_open())
    {
        Log(ERR, "file \"%s\" not found.", grammar_filepath.data());
        return;
    }
    std::set<std::string> all_syms;
    for (; getline(fin, buf); lines++)
    {
        int mid_pos = buf.find("->");
        if (mid_pos == -1)
        {
            Log(ERR, "表达式格式有误, 已忽略");
            continue;
        }
        std::string tmp;
        fft::T form;

        tmp = trim(buf.substr(0, mid_pos));
        form.first = tmp;
        this->left_part.push_back(tmp);
        this->Vns.insert(tmp);
        all_syms.insert(tmp);
        std::stringstream ss(trim(buf.substr(mid_pos + 2)));
        while (ss >> tmp)
        {
            form.second.push_back(tmp);
            all_syms.insert(tmp);
        }
        this->formula.push_back(form);
    }
    this->left_part.erase(std::unique(left_part.begin(), left_part.end()), left_part.end());
    std::set_difference(all_syms.begin(), all_syms.end(), Vns.begin(), Vns.end(), std::inserter(Vts, Vts.begin()));
    fin.close();
}

void FirstFollowTable::calFirst()
{
    int lines = this->formula.size();
    int pre = -1, now = 0;
    while (pre != now)
    {
        pre = now;
        for (int i = 0; i < lines; i++)
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
            now = 0;
            for (auto t : Vns)
            {
                now += (int)first[t].size();
            }
        }
    }
}

void FirstFollowTable::calFollow()
{
    int lines = this->formula.size();
    follow[*left_part.begin()].insert("#");
    int pre = -1, now = 0;
    while (pre != now)
    {
        pre = now;
        for (int i = 0; i < lines; i++)
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
        now = 0;
        for (auto n : Vns)
        {
            now += (int)follow[n].size();
        }
    }
}

void FirstFollowTable::calStrFirst()
{
    int lines = this->formula.size();
    for (int i = 0; i < lines; i++)
    {
        std::string form = getFormulaStr(formula[i].second);
        left_part.push_back(form);
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

FirstFollowTable *FirstFollowTable::run()
{
    this->read();

    this->calFirst();
    this->calFollow();
    this->calStrFirst();

    _runover = true;
    return this;
}

FirstFollowTable *FirstFollowTable::output()
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
    std::string buf;

    fprintf(fout, "FIRST集: \n");
    for (const auto &l : left_part)
    {
        buf.clear();
        buf += "FIRST(" + l + "): { ";
        for (const auto &r : this->first[l])
        {
            buf += r + ", ";
        }
        buf += " }";
        fprintf(fout, "%s\n", buf.data());
    }

    fprintf(fout, "FOLLOW集: \n");
    for (const auto &l : left_part)
    {
        buf.clear();
        buf += "FOLLOW(" + l + "): { ";
        for (const auto &r : this->follow[l])
        {
            buf += r + ", ";
        }
        buf += " }";
        fprintf(fout, "%s\n", buf.data());
    }

    fclose(fout);
    return this;
}

void FirstFollowTable::formulaDisplay()
{
    std::string buf;
    for (const auto &f : this->formula)
    {
        buf.clear();
        buf += f.first + " -> ";
        for (const auto &r : f.second)
        {
            buf += r + ", ";
        }
        Log(INFO, "%s", buf.data());
    }
}

void FirstFollowTable::vnsDisplay()
{
    std::string buf;
    buf += "Vns = { ";
    for (const auto &n : this->Vns)
    {
        buf += n + ", ";
    }
    buf += " }";
    Log(INFO, "%s", buf.data());
}

void FirstFollowTable::vtsDisplay()
{
    std::string buf;
    buf += "Vts = { ";
    for (const auto &t : this->Vts)
    {
        buf += t + ", ";
    }
    buf += " }";
    Log(INFO, "%s", buf.data());
}

std::string FirstFollowTable::getFormulaStr(const std::vector<std::string> &v)
{
    std::string res;
    for (const auto &f : v)
    {
        res += f + " ";
    }
    return res;
}