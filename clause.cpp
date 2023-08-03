//
// Created by natalia on 31.07.23.
//
#include "clause.h"

void clause::add_lit(lit lit) {
    lits.insert(lits.end(), lit);
}

bool clause::propagate(lit lit, watch_list *twoatch, assignment *assgn) {
    if (lits[watch1] == lit.neg_copy()) {
        int changed_watch = watch1;
        watch1 = watch2;
        watch2 = changed_watch;
    }

    // fully assigned (and satisfied)
    if (assgn->apply(lits[watch1]) == sat_bool::True) {
        twoatch->add_clause(lit, this); //??
        return true;
    }

    if (swap_watch2(twoatch, assgn)) {
        return true;
    }

    twoatch->add_clause(lit, this);
    return assgn->assign_and_enqueue(lits[watch1]);
}

bool clause::swap_watch2(watch_list *twoatch, assignment *assgn) {
    for (int i = watch2 + 1; i < lits.size(); i++) {
        if ((i != watch1) && (assgn->apply(lits[i]) != sat_bool::False)) {
            watch2 = i;
            twoatch->nadd_clause(lits[i], this);
            return true;
        }
    }
    for (int i = 0; i < watch2; i++) {
        if ((i != watch1) && (assgn->apply(lits[i]) != sat_bool::False)) {
            watch2 = i;
            twoatch->nadd_clause(lits[i], this);
            return true;
        }
    }
    if (logger::cond_log(logger::type::DEBUG_VERBOSE) && (assgn->apply(lits[watch2]) != sat_bool::False)) {
        logger::log(logger::type::DEBUG_VERBOSE,
                    "Somehow this variable is false and not false at the same time during propagation :(");
    }
    return false;
}
sat_bool clause::init(assignment *assgn) {
    vector<sat_bool> duplicates = vector<sat_bool>(assgn->get_lit_num());
    std::fill(duplicates.begin(), duplicates.end(), sat_bool::Undef);

    for (lit lit : lits) {
        sat_bool value = assgn->apply(lit);
        if (value == sat_bool::True) {
            return sat_bool::True;
        } else if (value == sat_bool::False) {
            duplicates[lit.get_id()] = sat_bool::False;
        } else {
            duplicates[lit.get_id()] = sat_bool::True;
        }
        if (duplicates[lit.get_nid()] != sat_bool::Undef) {
            return sat_bool::True;
        }
    }

    for (int i = 1; i < duplicates.size(); i++) {
        if (duplicates[i] == sat_bool::True) {
            lits.erase(unique(lits.begin(), lits.end()), lits.end());
        }
        if (duplicates[i] == sat_bool::False) {
            lits.erase(std::remove(lits.begin(), lits.end(), lit((int)i/2, i%2)), lits.end());
        }
    }

    if (lits.empty()) {
        return sat_bool::False;
    } else if (lits.size() == 1) {
        return assgn->assign_and_enqueue(lits[0]) ? sat_bool::True : sat_bool::False;
    } else {
        watch1 = 0;
        watch2 = 1;
        return sat_bool::Undef;
    }
}

sat_bool clause::init_learnt(lit watch, assignment *assgn, priority *prio) {
    if (lits.empty()) {
        return sat_bool::False;
    }

    if (lits.size() == 1) {
        return assgn->assign_and_enqueue(watch, this) ? sat_bool::True : sat_bool::False;
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
    return sat_bool::Undef;
}

void clause::calc_reason(lit of, vector<lit> *reason) {
    if (logger::cond_log(logger::type::ERROR) && !(of == lits[watch1]) && of.get_var() != 0) {
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

int clause::occurrences(int var) {
    int ret_val = 0;
    for (lit lit : lits) {
        if (lit.get_var() == var) {
            lit.is_neg() ? ret_val-- : ret_val++;
        }
    }
    return ret_val;
}

void clause::init_watch(watch_list *twoatch) {
    twoatch->nadd_clause(lits[watch1], this); //??
    twoatch->nadd_clause(lits[watch2], this);
}