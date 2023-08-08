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
    vector<vector<weak_ptr<clause>>> list;

    int lits_prev = 0;

public:
    watch_list() {
        list = vector<vector<weak_ptr<clause>>>();
    }

    void init(unsigned int size);

    void undo(lit lit);

    weak_ptr<clause> propagate(lit lit, assignment* assgn);

    void add_clause(lit lit, weak_ptr<clause> clause);

    void nadd_clause(lit lit, weak_ptr<clause> clause);

    bool remove_clause(lit lit, weak_ptr<clause> clause);

    bool nremove_clause(lit lit, weak_ptr<clause> clause);

};


#endif //SATSOLVER_WATCH_LIST_H
