//
// Created by natalia on 31.05.23.
//

#ifndef SATSOLVER_ASSIGNMENT_H
#define SATSOLVER_ASSIGNMENT_H


#include "sat_bool.h"
#include "exceptions/sat_exception.h"
#include "lit.h"
#include "logger.h"
#include "clause.h"
#include "watch_list.h"
#include <vector>
#include <queue>

class assignment {
private:
    std::vector<sat_bool> assgn;
    std::vector<lit> chrono_assgn;
    std::vector<int> level_sep;
    std::vector<int> assgn_levels;
    std::queue<lit> propagation;
    std::vector<clause*> reasons;
    int var_num;
public:
    assignment() {
        assgn = std::vector<sat_bool>();
        var_num = 0;
    }

    void init(int var);

    bool assign_and_enqueue(lit mk_true, clause* reason = nullptr);

    clause *propagate(watch_list *twoatch);

    sat_bool apply(lit lit);

    void undo(int var);

    void new_decision_level();

    sat_bool get_assignment(int var);

    void set_var_num(int num);

    int get_var_num();
};


#endif //SATSOLVER_ASSIGNMENT_H
