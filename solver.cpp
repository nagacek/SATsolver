//
// Created by natalia on 31.05.23.
//

#include "solver.h"

sat_bool solver::solve() {
    cnf_val.init_watches(&twoatch);
    prio.init(assgn.get_var_num());
    if (state == sat_bool::False) {
        return sat_bool::False;
    }
    while (true) {
        if (allAssigned()) {
            return sat_bool::True;
        } else {
            lit decided = prio.decide(&assgn, &cnf_val);
            assgn.new_decision_level();
            assgn.assign_and_enqueue(decided);

            clause* conflict = assgn.propagate(&twoatch);
            if (conflict != nullptr) {
                clause* learnt_clause = cnf_val.add_learnt_clause();
                lit asserting = {0, false};
                int backtrack_level = calc_reason(conflict, learnt_clause, &asserting);
                assgn.undo_until(backtrack_level);
                sat_bool value = learnt_clause->init_learnt(asserting, &assgn, &prio);
                if (value != sat_bool::Undef) {
                    if (value == sat_bool::False) {
                        return sat_bool::False;
                    }
                    cnf_val.reverse_last_learnt();
                }
                prio.update();
            }
        }
    }
}

bool solver::allAssigned() {
    for (int i = 1; i <= assgn.get_var_num(); i++) {
        if (assgn.get_assignment(i) == sat_bool::Undef) {
            return false;
        }
    }
    return true;
}

int solver::calc_reason(clause *conflict, clause *learnt, lit* asserting) {
    lit expansion = {0, false};
    int counter = 0;
    int max_level = 0;

    vector<bool> lits_seen = vector<bool>(assgn.get_var_num() + 1);
    std::fill(lits_seen.begin(), lits_seen.end(), false);

    vector<lit> inter_reason = vector<lit>();

    do {
        inter_reason.clear();
        conflict->calc_reason(expansion, &inter_reason);

        for (int i = 0; i < inter_reason.size(); i++) {
            lit new_expansion = inter_reason[i];
            int var = (int)new_expansion.get_var();
            if (!lits_seen[var]) {
                lits_seen[var] = true;
                int exp_level = assgn.get_level(new_expansion);
                if (exp_level == assgn.get_level()) {
                    counter++;
                } else if (exp_level > 0) {
                    learnt->add_lit(new_expansion.neg_copy());
                    max_level = max(max_level, exp_level);
                }
            }
        }

        do {
            expansion = assgn.get_last_assign();
            conflict = assgn.get_reason(expansion);
            assgn.undo_last();
        } while(!lits_seen[expansion.get_var()]);
        counter--;
    } while (counter > 0);

    *asserting = expansion.neg_copy();

    return max_level;
}

void solver::set_state(sat_bool st) {
    state = st;
}

