//
// Created by natalia on 31.05.23.
//

#include "cnf.h"

weak_ptr<clause> cnf::add_clause(const std::vector<lit> & lits) {
    return clauses.emplace_back(new clause(lits));
}

weak_ptr<clause> cnf::add_learnt_clause(const std::vector<lit> & lits) {
    return learnt_clauses.emplace_back(new clause(lits));
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

    for (shared_ptr<clause> & cl : clauses) {
        ret_val += cl->occurrences(var);
    }

    return ret_val;
}

void cnf::init_watches(watch_list *twoatch) {
    for (shared_ptr<clause> & cl : clauses) {
        cl->init_watch(twoatch);
    }
}
