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

void NFA::genMatrix(const closure_type& S){
    std::vector<closure_type> open;
    std::set<closure_type> close;
    std::map<closure_type, std::map<std::string, closure_type>> _matrix;
    std::map<closure_type, int> dict;
    auto get_n = [](closure_type c){static int n = 0; printf("%s: %d\n", convert(c).data(), n);return n++;};

    const auto &s = getEpsilonClosure(S);
    open.push_back(s);
    dict[s] = get_n(s);
    
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
                dict[g] = get_n(g);
            }
        }
    }

    for(const auto &[l, r]: _matrix){
        for(const auto &[ll, rr]: r){
            this->matrix[dict[l]][ll] = dict[rr];
        }
    }
}

NFA* NFA::run(){
    read();
    genMatrix(closure_type{"X"});
    return this;
}

NFA* NFA::output(){

    for(const auto &[l, r]: matrix){
        for(const auto &[ll, rr]: r){
            printf("%d, %s, %d\n", l, ll.data(), rr);
        }
    }
    
    return this;
}