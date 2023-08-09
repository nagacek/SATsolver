//
// Created by natalia on 31.05.23.
//

#include "cnf.h"

weak_ptr<clause> cnf::add_clause(const std::vector<lit> & lits) {
    return clauses.emplace_back(new clause(lits));
}

weak_ptr<clause> cnf::add_learnt_clause(const std::vector<lit> & lits, priority * prio) {
    prio->new_cla();
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

void cnf::occurrences(vector<int> &vector) {
    for (shared_ptr<clause> & cl : clauses) {
        cl->occurrences(vector);
    }
}

int cnf::find_learnt(weak_ptr<clause> conflict) {
    int i = 0;
    for (shared_ptr<clause> &cl : learnt_clauses) {
        if (cl == conflict.lock()) {
            return i;
        }
        i++;
    }
    return -1;
}

int cnf::get_learnt_num() {
    return (int)learnt_clauses.size();
}

int cnf::get_clause_num() {
    return clause_num;
}

void cnf::init_watches(watch_list *twoatch) {
    for (shared_ptr<clause> & cl : clauses) {
        cl->init_watch(twoatch);
    }
}

void cnf::prune_clauses(priority *prio, watch_list *twoatch) {
    double median = prio->calc_median();
    double threshold = prio->get_cla_thresh();

    int num = 0;
    for (auto it = learnt_clauses.begin(); it != learnt_clauses.end();) {
        if (it->use_count() < 2 && (prio->get_cla_prio(num) < median || prio->get_cla_prio(num) < threshold)) { // unit clauses??
            (*it)->cancel_watches(twoatch);
            it = learnt_clauses.erase(it);
            prio->delete_cla(num);
        } else {
            it++;
            num++;
        }
    }
}