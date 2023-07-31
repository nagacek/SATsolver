//
// Created by natalia on 05.06.23.
//

#include "watch_list.h"

// size == num of vars
void watch_list::init(unsigned size) {
    list = vector<vector<clause*>>(1 + 2 * size, vector<clause*>());
}

vector<clause*>* watch_list::get_clauses(lit lit) {
    return &list.at(lit.get_id());
}

void watch_list::undo(lit lit) {
    list[lit.get_id()] = vector<clause*>();
}

clause* watch_list::propagate(lit lit, assignment* assgn) {
    vector<clause*> temp_list = std::move(list[lit.get_id()]); //??
    undo(lit);
    for (int i = 0; i < temp_list.size(); i++) {
        if (!temp_list[i]->propagate(lit, this, assgn)) {
            for (int j = i + 1; j <= temp_list.size(); j++) {
                list[lit.get_id()].emplace_back(temp_list[j]);
            }
            return temp_list[i];
        }
    }
    return nullptr;
}

bool watch_list::nadd_clause(lit lit, clause* clause) {
    auto inner = list.at(lit.get_nid());
    if (std::find(inner.begin(), inner.end(), clause) != inner.end()) {
        inner.emplace_back(clause);
        return true;
    } else {
        return false;
    }
}

bool watch_list::add_clause(lit lit, clause* clause) {
    auto inner = list.at(lit.get_id());
    if (std::find(inner.begin(), inner.end(), clause) != inner.end()) {
        inner.emplace_back(clause);
        return true;
    } else {
        return false;
    }
}

