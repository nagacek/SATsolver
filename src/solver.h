//
// Created by natalia on 31.05.23.
//

#ifndef SATSOLVER_SOLVER_H
#define SATSOLVER_SOLVER_H


#include <chrono>
#include "assignment.h"
#include "cnf.h"
#include "watch_list.h"
#include "priority.h"

class solver {
    cnf cnf_val;
    assignment assgn;
    watch_list twoatch;
    priority prio;
    sat_bool state;

    // statistics
    double prop_time = 0;
    double reason_time = 0;
    double assert_time = 0;
    double prune_time = 0;
    double all_time = 0;
    double conf_no = 0;
    double conf_no_total = 0;
    double learnt_no = 0;
    double prop_time_total = 0;
    double reason_time_total = 0;
    double assert_time_total = 0;
    double prune_time_total = 0;
    double all_time_total = 0;
    double restarts = 0;

public:


    solver(double var_en, double var_dim, double cla_en, double cla_dim) {
        assgn = assignment();
        cnf_val = cnf();
        twoatch = watch_list();
        prio = priority(var_en, var_dim, cla_en, cla_dim);
        state = sat_bool::Undef;
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
    bool solve();

    sat_bool try_solve(int max_learnts, int max_conflicts);

    void set_state(sat_bool st);

    void do_stats();

    void do_total_stats();

    sat_bool init(long learnts);

    void preprocess();

private:

    bool allAssigned();

    int calc_reason(weak_ptr<clause> conflict, vector<lit> & learnt, lit* asserting);

    void reset_times();

};


#endif //SATSOLVER_SOLVER_H
