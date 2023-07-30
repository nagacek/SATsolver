//
// Created by natalia on 05.06.23.
//

#ifndef SATSOLVER_WATCH_LIST_H
#define SATSOLVER_WATCH_LIST_H


#include "clause.h"
#include "lit.h"
#include <algorithm>

class watch_list {
    vector<vector<clause*>> list;

public:
    // size == num of vars
    void init(unsigned size) {
        list = vector<vector<clause*>>(1 + 2 * size, vector<clause*>());
    }

    vector<clause*>* get_clauses(lit lit) {
        return &list.at(lit.get_id());
    }

    vector<clause*> assume(lit lit) {
        auto watchers = list.at(lit.get_nid());
        
        for()
    }

    bool nadd_clause(lit lit, clause* clause) {
        auto inner = list.at(lit.get_nid());
        if (std::find(inner.begin(), inner.end(), clause) != inner.end()) {
            inner.emplace_back(clause);
            return true;
        } else {
            return false;
        }
    }

     bool nremove_clause(lit lit, clause* clause) {
        auto inner = list.at(lit.get_nid());
        auto iter = std::find(inner.begin(), inner.end(), clause);
        if (iter != inner.end()) {
            inner.erase(iter);
            return true;
        } else {
            return false;
        }
    }
};


#endif //SATSOLVER_WATCH_LIST_H
