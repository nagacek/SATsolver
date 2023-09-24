//
// Created by natalia on 31.07.23.
//

#include "assignment.h"


bool assignment::assign_and_enqueue(lit mk_true, shared_ptr<clause> reason) {
    int var = (int) mk_true.get_var();
    if (assgn[var] != sat_bool::Undef) {
        if (apply(mk_true) == sat_bool::False) {
            log(logger::type::DEBUG_CONFLICT, "Literal " + mk_true.to_string() + " has produced a conflict.");
            return false;
        }
        return true;
    } else {
        if (logger::cond_log(logger::DEBUG) && !reason) {
            logger::log(logger::DEBUG, "Literal " + mk_true.to_string() + " is assigned");
        } else {
            logger::log(logger::DEBUG_VERBOSE,
                        std::string("Literal " + mk_true.to_string() + " is assigned")
                                .append(!reason ? "" : std::string(": ")
                                        .append(reason->to_string(false))));
        }
        assgn[var] = mk_true.is_neg() ? sat_bool::False : sat_bool::True;
        chrono_assgn.emplace_back(mk_true);
        assgn_levels[var] = (int) level_sep.size();
        reasons[var] = reason;
        propagation.emplace(mk_true);
        return true;
    }
}

weak_ptr<clause> assignment::propagate(watch_list *twoatch) {
    while (!propagation.empty()) {
        lit to_propagate = propagation.front();
        propagation.pop();

        weak_ptr<clause> conflict = twoatch->propagate(to_propagate, this);
        if (!conflict.expired()) {
            propagation = {}; //??
            return conflict;
        }
    }
    return {};
}

sat_bool assignment::apply(lit lit1) {
    lit repr = representants[lit1.get_var()];
    if (!(repr == lit{}) && repr.get_var() != lit1.get_var()) {
        return lit1.is_neg() ? !apply(repr) : apply(repr);
    }
    return lit1.is_neg() ? !assgn.at(lit1.get_var()) : assgn.at(lit1.get_var());
}

void assignment::undo_last(priority * prio) {
    lit lit = chrono_assgn.back();
    prio->save_phase(lit);
    chrono_assgn.pop_back();
    int var = (int) lit.get_var();
    assgn[var] = sat_bool::Undef;
    assgn_levels[var] = -1;
    reasons[var] = {};

    if (level_sep[level_sep.size() - 1] > ((int)chrono_assgn.size()) - 1) {
        level_sep.pop_back();
    }
}

void assignment::undo_until(int level, priority * prio) {
    logger::log(logger::DEBUG_CONFLICT, "Undo until level " + to_string(level));
    int i = (int) chrono_assgn.size();
    int until = level_sep[level];
    if (logger::cond_log(logger::DEBUG_VERBOSE)) {
        std::string log_val = "-undo-> ";
        for (; i > until; i--) {
            log_val += chrono_assgn.back().to_string() + ", ";
            undo_last(prio);
        }
        log_val.pop_back();
        log_val.pop_back();
        logger::log(logger::DEBUG_VERBOSE, log_val);
    } else {
        for (; i > until; i--) {
            undo_last(prio);
        }
    }
}

int assignment::get_level(lit lit) {
    return assgn_levels[lit.get_var()];
}

int assignment::get_level() {
    return (int) level_sep.size();
}

void assignment::new_decision_level() {
    level_sep.emplace_back(chrono_assgn.size());

    // ---- logging ----
    logger::log(logger::DEBUG_VERBOSE, "New decision level: " + to_string(level_sep.size()));
    if (logger::cond_log(logger::DEBUG_VERY_VERBOSE)) {
        std::string log_val = "Current assignment: \n";
        for (int i = 1; i < assgn_levels.size(); i++) {
            sat_bool assignment = this->get_assignment(i);
            lit l = lit(i, assignment == sat_bool::False);
            if (assignment == sat_bool::Undef) {
                log_val += "#" + l.to_string();
            } else if (assignment == sat_bool::False) {
                log_val += l.to_string() + "@" + to_string(get_level(l));
            } else {
                log_val += " " + l.to_string() + "@" + to_string(get_level(l));
            }
            log_val += "\n";
        }
        logger::log(logger::DEBUG_VERY_VERBOSE, log_val);
    }

}

sat_bool assignment::get_assignment(int var) {
    lit repr = representants[var];
    if (!(repr == lit{}) && repr.get_var() != var) {
        return apply({var, false});
    }
    return assgn.at(var);
}

void assignment::set_var_num(int num) {
    if (num <= 0)
        throw invalid_arg_exception(std::string("negative amount of variables"));
    var_num = num;
    assgn.resize(num + 1);
    representants.resize(num + 1);
    assgn_levels.resize(num + 1);
    reasons.resize(num + 1);
    std::fill(assgn_levels.begin(), assgn_levels.end(), -1);
    std::fill(assgn.begin(), assgn.end(), sat_bool::Undef);
    std::fill(reasons.begin(), reasons.end(), shared_ptr<clause>());
}

int assignment::get_var_num() { return var_num; }

lit assignment::get_last_assign() {
    return chrono_assgn.back();
}

weak_ptr<clause> assignment::get_reason(lit lit) {
    return reasons[lit.get_var()];
}

int assignment::get_lit_num() {
    return var_num * 2 + 1;
}

int assignment::get_assgn_num() {
    return chrono_assgn.size();
}

void assignment::set_representant(lit eq, lit repr) {
    representants[eq.get_var()] = eq.is_neg() ? repr.neg_copy() : repr;
}

lit assignment::get_representant(lit eq) {
    return has_representant(eq) ? (eq.is_neg() ? representants[eq.get_var()].neg_copy() : representants[eq.get_var()]) : eq;
}

int assignment::get_representant(int eq) {
    return has_representant(eq) ? (int)representants[eq].get_var() : (int)eq;
}

bool assignment::has_representant(lit eq) {
    return !(representants[eq.get_var()] == lit{});
}

bool assignment::has_representant(int eq) {
    return !(representants[eq] == lit{});
}
