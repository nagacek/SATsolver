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
    if (logger::cond_log(logger::type::DEBUG_VERBOSE) && (assgn->apply(lits[watch1]) != sat_bool::False)) {
        logger::log(logger::type::DEBUG_VERBOSE,
                    "Somehow this variable is false and not false at the same time during propagation :(");
    }
    return false;
}

bool clause::init_learnt(lit watch, assignment *assgn, priority *prio) {
    if (lits.empty()) {
        return false;
    }

    if (lits.size() == 1) {
        assgn->assign_and_enqueue(watch, this);
        return false;
    }
    lits.emplace_back(watch);
    learnt = true;
    int var = -1;
    int max_level = -1;
    for (int i = 0; i < lits.size(); i++) {
        int inter_level = assgn->get_level(lits[i]);
        if (lits[i] == watch) {
            watch1 = i;
        } else if (inter_level > max_level) {
            max_level = inter_level;
            var = i;
        }
        prio->enhance(lits[i].get_var());
    }
    watch2 = var;
    return true;
}

void clause::calc_reason(lit of, vector<lit> *reason) {
    if (logger::cond_log(logger::type::ERROR) && !(of == lits[watch1]) || of.get_var() == 0) {
        logger::log(logger::type::ERROR,
                    std::string("Given literal ").append((of.is_neg() ? "¬" : "")).append("[").append(
                            to_string(of.get_var())).append("] is not conflicting"));
    }
    for (int i = 0; i < lits.size(); i++) {
        if (i == watch1 && of == lits[watch1]) {
            continue;
        }
        reason->emplace_back(lits[i].neg_copy());
    }
}
