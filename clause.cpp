//
// Created by natalia on 31.07.23.
//
#include "clause.h"

void clause::add_lit(lit lit) {
    lits.insert(lits.end(), lit);
}

bool clause::propagate(lit lit, watch_list *twoatch, assignment *assgn) {
    if (lits[watch2] == lit.neg_copy()) {
        int changed_watch = watch2;
        watch2 = watch1;
        watch1 = changed_watch;
    }

    // fully assigned (and satisfied)
    if (assgn->apply(lits[watch2]) == sat_bool::True) {
        twoatch->add_clause(lit, this); //??
        return true;
    }

    if (swap_watch1(twoatch, assgn)) {
        return true;
    }

    twoatch->add_clause(lit, this);
    return assgn->assign_and_enqueue(lits[watch1]);
}

bool clause::swap_watch1(watch_list *twoatch, assignment *assgn) {
    for (int i = watch1 + 1; i < lits.size(); i++) {
        if ((i != watch2) && (assgn->apply(lits[i]) != sat_bool::False)) {
            watch1 = i;
            twoatch->nadd_clause(lits[i], this);
            return true;
        }
    }
    for (int i = 0; i < watch1; i++) {
        if ((i != watch2) && (assgn->apply(lits[i]) != sat_bool::False)) {
            watch1 = i;
            twoatch->nadd_clause(lits[i], this);
            return true;
        }
    }
    if (logger::LOG_LEVEL == logger::type::DEBUG_VERBOSE && (assgn->apply(lits[watch1]) != sat_bool::False)) {
        logger::log(logger::type::DEBUG_VERBOSE,
                    "Somehow this variable is false and not false at the same time during propagation :(");
    }
    return false;
}