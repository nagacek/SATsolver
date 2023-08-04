//
// Created by natalia on 31.07.23.
//

#include "assignment.h"

void assignment::init(int var) {
    assgn[var] = sat_bool::Undef;
}

bool assignment::assign_and_enqueue(lit mk_true, clause * const & reason) {
    int var = (int) mk_true.get_var();
    if (assgn[var] != sat_bool::Undef) {
        if (apply(mk_true) == sat_bool::False) {
            log(logger::type::DEBUG_VERBOSE, "Variable [" + to_string(var) + "] has produced a conflict.");
            return false;
        }
        return true;
    } else {
        assgn[var] = mk_true.is_neg() ? sat_bool::False : sat_bool::True;
        chrono_assgn.emplace_back(mk_true);
        assgn_levels[var] = (int) level_sep.size();
        reasons[var] = reason;
        propagation.emplace(mk_true);
        return true;
    }
}

clause* assignment::propagate(watch_list* twoatch) {
    while(!propagation.empty()) {
        lit to_propagate = propagation.front();
        propagation.pop();

        clause* conflict = twoatch->propagate(to_propagate, this);
        if (conflict != nullptr) {
            propagation = {}; //??
            return conflict;
        }
    }
    return nullptr;
}

sat_bool assignment::apply(lit lit) {
    return lit.is_neg() ? !assgn.at(lit.get_var()) : assgn.at(lit.get_var());
}

void assignment::undo_last() {
    lit lit = chrono_assgn.back();
    chrono_assgn.pop_back();
    int var = lit.get_var();
    assgn[var] = sat_bool::Undef;
    assgn_levels[var] = -1;
    reasons[var] = nullptr;

    if (level_sep[level_sep.size() - 1] > chrono_assgn.size() - 1) {
        level_sep.pop_back();
    }
}

void assignment::undo_until(int level) {
    int i = chrono_assgn.size();
    int until = level_sep[level];
    for (; i > until; i--) {
        undo_last();
    }
}

int assignment::get_level(lit lit) {
    return assgn_levels[lit.get_var()];
}

int assignment::get_level() {
    return (int)level_sep.size();
}

void assignment::new_decision_level() {
    level_sep.emplace_back(chrono_assgn.size());
}

sat_bool assignment::get_assignment(int var) {
    return assgn.at(var);
}

void assignment::set_var_num(int num) {
    if (num <= 0)
        throw invalid_arg_exception(std::string("negative amount of variables"));
    var_num = num;
    assgn.resize(num + 1);
    assgn_levels.resize(num + 1);
    reasons.resize(num + 1);
    std::fill(assgn_levels.begin(), assgn_levels.end(), -1);
    std::fill(assgn.begin(), assgn.end(), sat_bool::Undef);
    std::fill(reasons.begin(), reasons.end(), nullptr);
}

int assignment::get_var_num() { return var_num; }

lit assignment::get_last_assign() {
    return chrono_assgn.back();
}

clause* assignment::get_reason(lit lit) {
    return reasons[lit.get_var()];
}

int assignment::get_lit_num() {
    return var_num * 2 + 1;
}
