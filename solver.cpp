//
// Created by natalia on 31.05.23.
//

#include "solver.h"
#include "logger.h"

sat_bool solver::solve() {
    init_prio();
    while (true) {
        if (allAssigned()) {
            return sat_bool::True;
        } else {
            lit decided = decide();
            assgn.new_decision_level();
            assgn.assign_and_enqueue(decided);

            assgn.propagate(twoatch);
        }
    }
}

void solver::init_prio() {
    priority.resize(assgn.get_var_num() + 1);
}

lit solver::decide() {
    unsigned decided = 0;
    float decided_val = -1;
    for (int i = 1; i <= assgn.get_var_num(); i++) {
        float current_val = priority[i];
        if (current_val > decided_val && assgn.get_assignment(i) == sat_bool::Undef) {
            decided = i;
            decided_val = current_val;
        }
    }
    if (decided == 0) {
        logger::log(logger::type::ERROR,
                    "Decision procedure found no variable with highest priority even though there is no satisfying assignment.");
        exit(-1);
    }
    return {decided, false};
}

bool solver::allAssigned() {
    for (int i = 1; i <= assgn.get_var_num(); i++) {
        if (assgn.get_assignment(i) == sat_bool::Undef) {
            return false;
        }
    }
    return true;
}
