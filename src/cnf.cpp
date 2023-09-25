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

vector<weak_ptr<clause>> cnf::get_weak_copy() {
    vector<weak_ptr<clause>> ret_val{};
    ret_val.reserve(clauses.size());
    for (auto &ptr : clauses) {
        ret_val.push_back(ptr);
    }
    return ret_val;
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
    return clauses.size();
}

void cnf::init_watches(watch_list *twoatch) {
    for (shared_ptr<clause> & cl : clauses) {
        cl->init_watch(twoatch);
    }
}


void cnf::init_all_watches(watch_list * all_watches) {
    for (shared_ptr<clause> & cl : clauses) {
        cl->init_occurrences(all_watches);
    }
}

void cnf::delete_clause(weak_ptr<clause> to_delete) {
    auto it = find(clauses.begin(), clauses.end(),  to_delete.lock());
    if (it == clauses.end()) {
        logger::log(logger::ERROR, "Clause to be deleted is not there");
        exit(-1);
    }
    clauses.erase(it);
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

sat_bool cnf::init(assignment * assgn, double learnts) {
    learnt_clauses.reserve((long)learnts);
    for (auto it = clauses.begin(); it != clauses.end();) {
        sat_bool sat_value = (*it)->init(assgn);
        if (sat_value != sat_bool::Undef) {
            if (sat_value == sat_bool::False) {
                return sat_bool::False;
            }
            it = clauses.erase(it);
            clause_num--;
            continue;
        }
        it++;
    }
    return sat_bool::Undef;
}

void cnf::find_binary_clauses(vector<weak_ptr<clause>>& list) {
    for (auto it = clauses.begin(); it != clauses.end(); it++) {
        shared_ptr<clause> curr = (*it);
        if (curr->get_size() == 2) {
            (&list)->push_back(curr);
        }
    }
}
