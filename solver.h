//
// Created by natalia on 31.05.23.
//

#ifndef SATSOLVER_SOLVER_H
#define SATSOLVER_SOLVER_H


#include "assignment.h"
#include "cnf.h"
#include "watch_list.h"

class solver {
    cnf cnf_val;
    assignment assgn;
    watch_list twoatch;
    vector<float> priority;
public:
    solver() {
        assgn = assignment();
        cnf_val = cnf();
        twoatch = watch_list();
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
    void propagate(lit p);
};


#endif //SATSOLVER_SOLVER_H
