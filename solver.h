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
    float variable_enhance;
    float variable_diminish;


public:
    solver(float var_en, float var_dim) {
        assgn = assignment();
        cnf_val = cnf();
        twoatch = watch_list();

        priority = vector<float>();
        variable_enhance = var_en;
        variable_diminish = var_dim;
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

    void init_prio();

    lit decide();

    bool allAssigned();
};


#endif //SATSOLVER_SOLVER_H
