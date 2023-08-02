//
// Created by natalia on 31.05.23.
//
#include <vector>
#include "lit.h"

#ifndef SATSOLVER_CLAUSE_H
#define SATSOLVER_CLAUSE_H

class watch_list;
class assignment;
class priority;
class clause {
private:
    std::vector<lit> lits;
    int watch1;
    int watch2;
    bool learnt;
public:
    clause() {
        lits = std::vector<lit>();
        watch1 = -1;
        watch2 = -1;
        learnt =  false;
    }

    void add_lit(lit lit);

    bool propagate(lit lit, watch_list* twoatch, assignment* assgn);

    bool swap_watch1(watch_list *twoatch, assignment *assgn);

    void init_watch();

    bool init_learnt(lit watch, assignment* assgn, priority *prio);

    void calc_reason(lit of, vector<lit> *reason);

};
#include "assignment.h"
#include "priority.h"
#include "watch_list.h"

#endif //SATSOLVER_CLAUSE_H
