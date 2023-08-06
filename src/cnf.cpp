//
// Created by natalia on 31.05.23.
//

#include "cnf.h"

clause* cnf::add_clause() {
    auto new_clause = clauses.insert(clauses.end(), clause());
    return new_clause.base();
}

clause* cnf::add_learnt_clause() {
    return &learnt_clauses.emplace_back();
}

void cnf::set_clause_num(int num) {
    if (num <= 0)
        throw invalid_arg_exception(std::string("negative number of clauses"));
    clause_num = num;
}

void cnf::reverse_last_learnt() {
    learnt_clauses.pop_back();
}

void cnf::reverse_last() {
    clauses.pop_back();
    clause_num--;
}

int cnf::occurrences(int var) {
    int ret_val = 0;

    for (clause &cl : clauses) {
        ret_val += cl.occurrences(var);
    }

    return ret_val;
}

void cnf::init_watches(watch_list *twoatch) {
    for (clause &cl : clauses) {
        cl.init_watch(twoatch);
    }
}
