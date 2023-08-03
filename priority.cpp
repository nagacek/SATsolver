//
// Created by natalia on 02.08.23.
//

#include "priority.h"
#include "cnf.h"

void priority::init(int var_num) {
    prio.resize(var_num + 1);
}

lit priority::decide(assignment *assgn, cnf *cnf) {
    int decided = 0;
    double decided_val = -1;
    for (int i = 1; i < prio.size(); i++) {
        double current_val = prio[i];
        if (current_val > decided_val && assgn->get_assignment(i) == sat_bool::Undef) {
            decided = i;
            decided_val = current_val;
        }
    }
    if (decided == 0) {
        log(logger::type::ERROR,
            "Decision procedure found no variable with highest priority even though there is no satisfying assignment.");
        exit(-1);
    }

    return {decided, cnf->occurrences(decided) < 0};
}

void priority::enhance(int var) {
    if (prio[var] += variable_enhance > 1e100) {

    }
}

void priority::update() {
    variable_enhance *= variable_diminish;
}
