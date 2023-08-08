//
// Created by natalia on 05.06.23.
//

#ifndef SATSOLVER_WATCH_LIST_H
#define SATSOLVER_WATCH_LIST_H


#include "lit.h"
#include <algorithm>
#include "clause.h"
#include "assignment.h"

class watch_list {
    vector<vector<clause*>> list;

    int lits_prev = 0;

public:
    watch_list() {
        list = vector<vector<clause*>>();
    }

    void init(unsigned int size);

    vector<clause *> *get_clauses(lit lit);

    void undo(lit lit);

    clause *propagate(lit lit, assignment* assgn);

    bool add_clause(lit lit, clause * const &clause);

    bool nadd_clause(lit lit, clause * const &clause);

    bool remove_clause(lit lit, clause * const &clause);

    bool nremove_clause(lit lit, clause *const &clause);

    void debug();
};


#endif //SATSOLVER_WATCH_LIST_H
