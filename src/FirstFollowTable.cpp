#include "FirstFollowTable.h"
#include "Logging.h"

FirstFollowTable::FirstFollowTable(std::string filename)
{
    this->filename = filename;
    std::fstream fin(this->filename);
    std::string tmp;
    this->index = 0;
    while (getline(fin, tmp))
    {
        this->index++;
    }
    this->Vns.insert("#");
    this->exps = new std::pair<std::string, std::vector<std::string>>[this->index];
    this->index = 0;
}

bool FirstFollowTable::isTerminalChar(const std::string &s)
{
    return (this->Vts.count(s) == 1);
}

void display(std::set<std::string> s)
{
    for (auto it = s.begin(); it != s.end(); ++it)
    {
        std::cout << *it;
    }
    std::cout << std::endl;
}

void FirstFollowTable::readExps()
{
    std::string str;
    std::fstream fin(this->filename);
    std::set<std::string> all_lit;
    for (; getline(fin, str); this->index++)
    {
        if ((int)str.find("#") != -1)
        {
            break;
        }
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
        this->exps[index].first = str.substr(sta_pos, end_pos - sta_pos);
        this->Vns.insert(exps[index].first);
        this->_left.push_back(exps[index].first);

        sta_pos = mid_pos + 2;
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
                        continue;
                    }
                    this->exps[index].second.push_back(ns);
                    all_lit.insert(ns);
                }
            }
        }
    }
    _left.erase(std::unique(_left.begin(), _left.end()), _left.end());
    std::set_difference(all_lit.begin(), all_lit.end(), Vns.begin(), Vns.end(), std::inserter(Vts, Vts.begin()));
    fin.close();
}

void FirstFollowTable::calFirst()
{
    int pre = -1, now = 0;
    while (pre != now)
    {
        pre = now;
        for (int i = 0; i < index; i++)
        {
            std::string str = exps[i].first;
            std::vector<std::string> element = exps[i].second;
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
    follow[*_left.begin()].insert("#");
    int pre = -1, now = 0;
    while (pre != now)
    {
        pre = now;
        for (int i = 0; i < index; i++)
        {
            std::string str = exps[i].first;
            std::vector<std::string> element = exps[i].second;
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

void FirstFollowTable::calAll()
{
    calFirst();
    calFollow();
}

void FirstFollowTable::displayFirst()
{
    std::cout << "---first---\n";
    for (auto n : _left)
    {
        printf("FIRST(%s): { ", n.data());
        for (auto t : first[n])
        {
            std::cout << t << ", ";
        }
        std::cout << " }" << std::endl;
    }
}

void FirstFollowTable::displayFollow()
{
    std::cout << "---follow---\n";
    for (auto n : _left)
    {
        printf("FOLLOW(%s): { ", n.data());
        for (auto t : follow[n])
        {
            std::cout << t << ", ";
        }
        std::cout << " }" << std::endl;
    }
}

void FirstFollowTable::showAll()
{
    printf("EXPS:\n");
    for (int i = 0; i < index; i++)
    {
        std::cout << exps[i].first << " -> ";
        for (auto s : exps[i].second)
        {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }
    printf("Vts(%ld): { ", Vts.size());
    for (auto t : Vts)
    {
        std::cout << t << " ";
    }
    std::cout << " }" << std::endl;
    printf("Vns(%ld): { ", Vns.size());
    for (auto n : Vns)
    {
        std::cout << n << " ";
    }
    std::cout << " }" << std::endl;
}