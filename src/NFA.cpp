#include "NFA.h"

void NFA::read(){
    std::string buf;
    std::ifstream fin(this->input_filepath);
    if(!fin.is_open())
    {
        Log(ERR, "file \"%s\" not found.", input_filepath.data());
        return;
    }

    while(getline(fin, buf)){
        std::stringstream ss(buf);
        std::string x, y, cond;
        ss >> x >> y >> cond;
        this->nfa[x][y].insert(cond);
        this->states.insert(x);
        this->states.insert(y);
        this->syms.insert(cond);
    }
}

const closure_type NFA::getSingleClosure(const closure_type& S, const std::string& sym){
    const auto &_ = [this](const std::string& I, const std::string& sym){
        closure_type ans;
        for(const auto &[l, r]: this->nfa[I]){
            if(r.count(sym)){
                ans.insert(l);
            }
        }
        return ans;
    };

    closure_type ans;
    for(const auto &s: S){
        const auto &__ = _(s, sym);
        ans.insert(__.begin(), __.end());
    }
    return ans;
}

const closure_type NFA::getEpsilonClosure(const closure_type& S){
    const auto &_ = [this](const std::string& I){
        closure_type ans;
        std::function<void(const std::string&)> __ = [&](const std::string& I){
            ans.insert(I);
            for(const auto &[l, r]: this->nfa[I]){
                if(r.count("$")){
                    ans.insert(l);
                    __(l);
                }
            }
        };
        __(I);
        return ans;
    };

    closure_type ans;
    for(const auto &s: S){
        const auto &__ = _(s);
        ans.insert(__.begin(), __.end());
    }
    return ans;
}



const closure_type NFA::Ix(const closure_type& I, std::string x){
    return getEpsilonClosure(getSingleClosure(I, x));
}

void NFA::determinNFA(){
    std::vector<closure_type> open;
    std::set<closure_type> close;
    std::map<closure_type, std::map<std::string, closure_type>> _matrix;
    std::map<closure_type, std::string> dict;
    auto get_n = []{static int n = 0; return std::to_string(n++);};

    const auto &s = getEpsilonClosure(closure_type{"X"});
    open.push_back(s);
    dict[s] = get_n();
    
    while(!open.empty()){
        const auto I = open.back();
        open.pop_back();
        close.insert(I);
        for(const auto &x: this->syms){
            if(x == "$") continue;
            const auto g = Ix(I, x);
            _matrix[I][x] = g;
            if(std::find(open.begin(), open.end(), g) == open.end() && !close.count(g)){
                open.push_back(g);
                close.insert(g);
                dict[g] = get_n();
            }
        }
    }

    this->states.clear();
    for(const auto &[_, v]: dict){
        this->states.insert(v);
    }
    this->syms.erase("$");

    for(const auto &[l, r]: _matrix){
        for(const auto &[ll, rr]: r){
            this->matrix[dict[l]][ll] = dict[rr];
        }
    }
}

const std::pair<closure_type, closure_type> NFA::move(const closure_type& S, const std::string& sym){
    std::pair<closure_type, closure_type> ans;
    for(const auto &s: S){
        const auto &tmp = this->matrix[s][sym];
        if(S.count(tmp)){
            ans.first.insert(s);
        }
        else{
            ans.second.insert(s);
        }
    }
    return ans;
}

void NFA::minimizeDFA(const closure_type& start, const closure_type& end){

    const auto &_ = [this](const closure_type& S){
        std::set<closure_type> ans;
        std::function<void(const closure_type&)> __ = [&](const closure_type& S){
            // std::cout << convert(S) << std::endl;
            if(S.size() <= 1){
                ans.insert(S);
                return;
            }
            bool is_same_partition = true;
            for(const auto &sym: this->syms){
                const auto &tmp = move(S, sym);
                if(!tmp.first.empty() && !tmp.second.empty()){
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
            if(is_same_partition){
                ans.insert(S);
            }
        };
        __(S);
        return ans;
    };

    std::set<closure_type> ans;
    const auto &_start = _(start);
    ans.insert(_start.begin(), _start.end());
    const auto &_end = _(end);
    ans.insert(_end.begin(), _end.end());
    std::cout << "ans size is: " << ans.size() << std::endl;
    for(const auto &a: ans){
        std::cout << convert(a) << std::endl;
    }

    std::map<std::string, std::string> dict;
    auto get_n = []{static int n = 0; return std::to_string(n++);};

    for(const auto &a: ans){
        auto t = get_n();
        for(const auto &c: a){
            dict[c] = t;
        }
    }

    for(const auto &[l, r]: this->matrix){
        for(const auto &[ll, rr]: r){
            this->dfa[dict[l]][ll] = dict[rr];
        }
    }

    for(const auto &[l, r]: this->dfa){
        for(const auto &[ll, rr]: r){
            printf("%s, %s, %s\n", l.data(), ll.data(), rr.data());
        }
    }

    // std::vector<closure_type> open, close, ans;
    // open.push_back(start);
    // open.push_back(end);
    // printf("open\n");
    // for(auto i: open){
    //     std::cout << convert(i) << std::endl;
    // }
    // while(!open.empty()){
    //     auto &now = open.back();
    //     open.pop_back();
    //     if(std::find(close.begin(), close.end(), now) == close.end()){
    //         continue;
    //     }
    //     bool flag = false;
    //     for(const auto &sym: syms){
    //         const auto &tmp = move(now, sym);
    //         if(!tmp.second.empty()){
    //             flag = true;
    //             close.push_back(now);
    //             open.push_back(tmp.first);
    //             open.push_back(tmp.second);
    //         }
    //     }
    //     if(!flag){
    //         ans.push_back(now);
    //     }
    // }
    // printf("open\n");
    // for(auto i: open){
    //     std::cout << convert(i) << std::endl;
    // }
    // printf("close\n");
    // for(auto i: close){
    //     std::cout << convert(i) << std::endl;
    // }
    // printf("ans\n");
    // for(auto i: ans){
    //     std::cout << convert(i) << std::endl;
    // }
}

NFA* NFA::run(){
    read();
    determinNFA();
    return this;
}

NFA* NFA::output(){

    for(const auto &[l, r]: matrix){
        for(const auto &[ll, rr]: r){
            printf("%s, %s, %s\n", l.data(), ll.data(), rr.data());
        }
    }
    
    return this;
}