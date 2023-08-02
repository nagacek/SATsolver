//
// Created by natalia on 31.05.23.
//

#ifndef SATSOLVER_SOLVER_H
#define SATSOLVER_SOLVER_H


#include "assignment.h"
#include "cnf.h"
#include "watch_list.h"
#include "priority.h"

class solver {
    cnf cnf_val;
    assignment assgn;
    watch_list twoatch;
    priority prio;


public:
    solver(float var_en, float var_dim) {
        assgn = assignment();
        cnf_val = cnf();
        twoatch = watch_list();
        prio = priority(var_en, var_dim);
    }
    assignment* get_assignment() {
        return &assgn;
    }
    cnf* get_formula() {
        return &cnf_val;
    }
    watch_list* get_watch_list() {
        return &twoatch;
    }
    sat_bool solve();

private:

    bool allAssigned();

    int calc_reason(clause *conflict, clause *learnt, lit* asserting);
};


#endif //SATSOLVER_SOLVER_H
