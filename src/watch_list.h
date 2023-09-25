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

    int lits_prev = 0;

protected:
    vector<vector<weak_ptr<clause>>> list;
public:
    unsigned prop;
    watch_list() {
        list = vector<vector<weak_ptr<clause>>>();
        prop = 0;
    }

    void undo(lit lit);

    virtual weak_ptr<clause> propagate(lit lit, assignment* assgn);

    void add_clause(lit lit, weak_ptr<clause> clause);

    void nadd_clause(lit lit, weak_ptr<clause> clause);

    bool remove_clause(lit lit, weak_ptr<clause> clause);

    bool nremove_clause(lit lit, weak_ptr<clause> clause);

    void init(unsigned int size);

    virtual void notify(weak_ptr<clause> clause);

    void log_prop();
};


#endif //SATSOLVER_WATCH_LIST_H
