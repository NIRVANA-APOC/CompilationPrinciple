#include "FA.h"

void FA::read()
{
    std::string buf;
    std::ifstream fin(this->input_filepath);
    if (!fin.is_open())
    {
        Log(ERR, "file \"%s\" not found.", input_filepath.data());
        return;
    }

    while (getline(fin, buf))
    {
        std::stringstream ss(buf);
        std::string x, y, cond;
        ss >> x >> y >> cond;
        this->nfa[x][y].insert(cond);
        this->states.insert(x);
        this->states.insert(y);
        this->syms.insert(cond);
    }
}

const closure FA::getSingleClosure(const closure &S, const std::string &sym)
{
    const auto &_ = [this](const std::string &I, const std::string &sym)
    {
        closure ans;
        for (const auto &[l, r] : this->nfa[I])
        {
            if (r.count(sym))
            {
                ans.insert(l);
            }
        }
        return ans;
    };

    closure ans;
    for (const auto &s : S)
    {
        const auto &__ = _(s, sym);
        ans.insert(__.begin(), __.end());
    }
    return ans;
}

const closure FA::getEpsilonClosure(const closure &S)
{
    const auto &_ = [this](const std::string &I)
    {
        closure ans;
        std::function<void(const std::string &)> __ = [&](const std::string &I)
        {
            ans.insert(I);
            for (const auto &[l, r] : this->nfa[I])
            {
                if (r.count("$"))
                {
                    ans.insert(l);
                    __(l);
                }
            }
        };
        __(I);
        return ans;
    };

    closure ans;
    for (const auto &s : S)
    {
        const auto &__ = _(s);
        ans.insert(__.begin(), __.end());
    }
    return ans;
}

const closure FA::Ix(const closure &I, std::string x)
{
    return getEpsilonClosure(getSingleClosure(I, x));
}

void FA::determinNFA()
{
    std::vector<closure> open;
    std::set<closure> close;
    std::map<closure, std::map<std::string, closure>> _matrix;
    std::map<closure, std::string> dict;
    auto get_n = []
    {static int n = 0; return std::to_string(n++); };

    const auto &s = getEpsilonClosure(closure{"X"});
    open.push_back(s);
    dict[s] = get_n();

    while (!open.empty())
    {
        const auto I = open.back();
        open.pop_back();
        close.insert(I);
        for (const auto &x : this->syms)
        {
            if (x == "$")
                continue;
            const auto g = Ix(I, x);
            _matrix[I][x] = g;
            if (std::find(open.begin(), open.end(), g) == open.end() && !close.count(g))
            {
                open.push_back(g);
                close.insert(g);
                dict[g] = get_n();
            }
        }
    }

    this->states.clear();
    for (const auto &[_, v] : dict)
    {
        this->states.insert(v);
    }
    this->syms.erase("$");

    for (const auto &[l, r] : _matrix)
    {
        for (const auto &[ll, rr] : r)
        {
            this->matrix[dict[l]][ll] = dict[rr];
        }
    }
}

const std::pair<closure, closure> FA::move(const closure &S, const std::string &sym)
{
    std::pair<closure, closure> ans;
    for (const auto &s : S)
    {
        const auto &tmp = this->matrix[s][sym];
        if (S.count(tmp))
        {
            ans.first.insert(s);
        }
        else
        {
            ans.second.insert(s);
        }
    }
    return ans;
}

void FA::minimizeDFA(const closure &start, const closure &end)
{

    const auto &_ = [this](const closure &S)
    {
        std::set<closure> ans;
        std::function<void(const closure &)> __ = [&](const closure &S)
        {
            // std::cout << convert(S) << std::endl;
            if (S.size() <= 1)
            {
                ans.insert(S);
                return;
            }
            bool is_same_partition = true;
            for (const auto &sym : this->syms)
            {
                const auto &tmp = move(S, sym);
                if (!tmp.first.empty() && !tmp.second.empty())
                {
                    // 不属于同一个划分
                    // std::cout << convert(S) << sym << std::endl;
                    // std::cout << "first" << convert(tmp.first) << std::endl;
                    // std::cout << "second" << convert(tmp.second) << std::endl;
                    is_same_partition = false;
                    __(tmp.first);
                    __(tmp.second);
                    return;
                }
            }
            if (is_same_partition)
            {
                ans.insert(S);
            }
        };
        __(S);
        return ans;
    };

    std::set<closure> ans;
    const auto &_start = _(start);
    ans.insert(_start.begin(), _start.end());
    const auto &_end = _(end);
    ans.insert(_end.begin(), _end.end());
    std::cout << "ans size is: " << ans.size() << std::endl;
    for (const auto &a : ans)
    {
        std::cout << convert(a) << std::endl;
    }

    std::map<std::string, std::string> dict;
    auto get_n = []
    {static int n = 0; return std::to_string(n++); };

    for (const auto &a : ans)
    {
        auto t = get_n();
        for (const auto &c : a)
        {
            dict[c] = t;
        }
    }

    for (const auto &[l, r] : this->matrix)
    {
        for (const auto &[ll, rr] : r)
        {
            this->dfa[dict[l]][ll] = dict[rr];
        }
    }

    for (const auto &[l, r] : this->dfa)
    {
        for (const auto &[ll, rr] : r)
        {
            printf("%s, %s, %s\n", l.data(), ll.data(), rr.data());
        }
    }
}

FA *FA::run()
{
    read();
    determinNFA();
    return this;
}

FA *FA::output()
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
    for (const auto &[l, r] : matrix)
    {
        for (const auto &[ll, rr] : r)
        {
            fprintf(fout, "%s, %s, %s\n", l.data(), ll.data(), rr.data());
        }
    }
    fclose(fout);
    return this;
}