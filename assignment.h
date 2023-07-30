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
    int var_num;
public:
    assignment() {
        assgn = std::vector<sat_bool>();
        var_num = 0;
    }

    void init(int var) {
        assgn[var] = sat_bool::Undef;
    }

    bool assign_and_enqueue(lit mk_true) {
        int var = (int) mk_true.get_var();
        if (assgn[var] != sat_bool::Undef) {
            if (apply(mk_true) == sat_bool::False) {
                logger::log(logger::type::DEBUG_VERBOSE, "Variable [" + to_string(var) + "] has produced a conflict.");
                return false;
            }
            return true;
        } else {
            assgn[var] = mk_true.is_neg() ? sat_bool::False : sat_bool::True;
            chrono_assgn.emplace_back(mk_true);
            assgn_levels[var] = (int) level_sep.size();
            propagation.emplace(mk_true);
            return true;
        }
    }

    clause* propagate(watch_list* twoatch) {
        while(!propagation.empty()) {
            lit to_propagate = propagation.front();
            propagation.pop();

            vector<clause*> list = std::move(*twoatch->get_clauses(to_propagate)); //??
            twoatch->undo(to_propagate);
            for (int i = 0; i < list.size(); i++) {

            }
        }
        return nullptr;
    }

    sat_bool apply(lit lit) {
        return lit.is_neg() ? !assgn.at(lit.get_var()) : assgn.at(lit.get_var());
    }

    void undo(int var) {
        assgn[var] = sat_bool::Undef;
        assgn_levels[var] = -1;
    }

    void new_decision_level() {
        level_sep.emplace_back(chrono_assgn.size());
    }

    sat_bool get_assignment(int var) {
        return assgn.at(var);
    }

    void set_var_num(int num) {
        if (num <= 0)
            throw invalid_arg_exception(std::string("negative amount of variables"));
        var_num = num;
        assgn.resize(num + 1);
        assgn_levels.resize(num + 1);
        std::fill(assgn_levels.begin(), assgn_levels.end(), -1);
    }

    int get_var_num() { return var_num; }

};


#endif //SATSOLVER_ASSIGNMENT_H
