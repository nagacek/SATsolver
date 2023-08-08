//
// Created by natalia on 05.06.23.
//

#include "watch_list.h"

// size == num of vars
void watch_list::init(unsigned size) {
    list = vector<vector<weak_ptr<clause>>>(1 + 2 * size, vector<weak_ptr<clause>>());
}

void watch_list::undo(lit lit) {
    list[lit.get_id()] = vector<weak_ptr<clause>>();
}

weak_ptr<clause> watch_list::propagate(lit lit, assignment *assgn) {
    logger::log(logger::DEBUG_VERBOSE, lit.to_string() + " is propagated");

    vector<weak_ptr<clause>> temp_list = list[lit.get_id()]; //??
    undo(lit);
    for (int i = 0; i < temp_list.size(); i++) {
        if (!temp_list[i].expired()) {
            auto locked = temp_list[i].lock();
            if (!locked->propagate(lit, this, assgn)) {
                auto it = (list.begin() + lit.get_id());
                for (int j = i + 1; j < temp_list.size(); j++) {
                    it->insert(it->end(), temp_list[j]);
                }
                return temp_list[i];
            }
        }
    }
    return {};
}


void watch_list::nadd_clause(lit lit, weak_ptr<clause> clause) {
    (list.begin()+lit.get_nid())->push_back(clause);
}

void watch_list::add_clause(lit lit, weak_ptr<clause> clause) {
    (list.begin()+lit.get_id())->push_back(clause);
}

bool watch_list::remove_clause(lit lit, clause *const &clause) {
    auto it = list.begin() + lit.get_id();
    auto it_found = std::find(it->begin(), it->end(), clause);
    if (it_found != it->end()) {
        it->erase(it_found);
        return true;
    } else {
        return false;
    }
}

bool watch_list::nremove_clause(lit lit, clause *const &clause) {
    debug();
    if (lit.get_var() == 224) {
        int i = 5;
    }
    auto it = list.begin() + lit.get_nid();
    auto it_found = std::find(it->begin(), it->end(), clause);
    if (logger::cond_log(logger::DEBUG)) {
        logger::log(logger::DEBUG, lit.neg_copy().to_string() + " contains:");
        for (auto it2 = it->begin(); it2 != it->end(); it2++) {
            logger::log(logger::DEBUG, "-> " + (*it2)->to_string(true));
        }
    }
    if (it_found != it->end()) {
        it->erase(it_found);
        return true;
    } else {
        return false;
    }
}

void watch_list::debug() {
    for (auto & it : list[481]) {
        logger::log(logger::ERROR, it->to_string(true));
    }
}

