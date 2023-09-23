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
#include "priority.h"
#include <vector>
#include <queue>

class assignment {
private:
    std::vector<sat_bool> assgn;
    std::vector<lit> representants;
    std::vector<lit> chrono_assgn;
    std::vector<int> level_sep;
    std::vector<int> assgn_levels;
    std::queue<lit> propagation;
    std::vector<shared_ptr<clause>> reasons;
    int var_num;
public:
    assignment() {
        assgn = std::vector<sat_bool>();
        var_num = 0;
    }

    bool assign_and_enqueue(lit mk_true, shared_ptr<clause> reason = {});

    weak_ptr<clause> propagate(watch_list *twoatch);

    sat_bool apply(lit lit);

    void undo_last(priority * prio);

    void undo_until(int level, priority * prio);

    void new_decision_level();

    int get_level(lit lit);

    int get_level();

    sat_bool get_assignment(int var);

    void set_var_num(int num);

    int get_var_num();

    int get_lit_num();

    int get_assgn_num();

    lit get_last_assign();

    weak_ptr<clause> get_reason(lit lit);

    void set_representant(lit eq, lit repr);
};


#endif //SATSOLVER_ASSIGNMENT_H
