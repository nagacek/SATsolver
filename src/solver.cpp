//
// Created by natalia on 31.05.23.
//

#include "solver.h"

sat_bool solver::solve() {
    auto t0_0 = std::chrono::high_resolution_clock::now();
    logger::log(logger::INFO, "Initializing");
    cnf_val.init_watches(&twoatch);
    prio.init(assgn.get_var_num());
    logger::log(logger::INFO, "Solving");
    if (state == sat_bool::False) {
        return sat_bool::False;
    }

    auto t0_1 = std::chrono::high_resolution_clock::now();
    if (logger::cond_log(logger::INFO)) {
        all_time += ((std::chrono::duration<double, std::milli>) (t0_1 - t0_0)).count();
        all_time_total += ((std::chrono::duration<double, std::milli>) (t0_1 - t0_0)).count();
    }

    while (true) {
        t0_0 = std::chrono::high_resolution_clock::now();
        auto t1 = std::chrono::high_resolution_clock::now();
        clause *conflict = assgn.propagate(&twoatch);
        auto t2 = std::chrono::high_resolution_clock::now();
        if (logger::cond_log(logger::INFO)) {
            prop_time += ((std::chrono::duration<double, std::milli>) (t2 - t1)).count();
            prop_time_total += ((std::chrono::duration<double, std::milli>) (t2 - t1)).count();
        }

        if (conflict != nullptr) {
            if (logger::cond_log(logger::INFO)) {
                conf_no++;
            }
            if (assgn.get_level() == 0) {
                return sat_bool::False;
            }

            clause *learnt_clause = cnf_val.add_learnt_clause();
            lit asserting = {0, false};

            auto t3 = std::chrono::high_resolution_clock::now();
            int backtrack_level = calc_reason(conflict, learnt_clause, &asserting);
            auto t4 = std::chrono::high_resolution_clock::now();
            if (logger::cond_log(logger::INFO)) {
                reason_time += ((std::chrono::duration<double, std::milli>) (t4 - t3)).count();
                reason_time_total += ((std::chrono::duration<double, std::milli>) (t4 - t3)).count();
            }

            assgn.undo_until(backtrack_level);
            sat_bool value = learnt_clause->init_learnt(asserting, &assgn, &prio, &twoatch);

            if (value != sat_bool::Undef) {
                if (value == sat_bool::False) {
                    return sat_bool::False;
                }
                cnf_val.reverse_last_learnt();
            } else if (logger::cond_log(logger::INFO)){
                learnt_no++;
            }

            logger::log(logger::DEBUG_VERBOSE, "Learnt reason added is " + learnt_clause->to_string(true));

            auto t5 = std::chrono::high_resolution_clock::now();
            prio.update();
            auto t6 = std::chrono::high_resolution_clock::now();
            if (logger::cond_log(logger::INFO)) {
                assert_time += ((std::chrono::duration<double, std::milli>) (t6 - t5)).count();
                assert_time_total += ((std::chrono::duration<double, std::milli>) (t6 - t5)).count();
            }

            do_stats();
        } else {
            if (allAssigned()) {
                return sat_bool::True;
            }

            auto t7 = std::chrono::high_resolution_clock::now();
            lit decided = prio.decide(&assgn, &cnf_val);
            auto t8 = std::chrono::high_resolution_clock::now();
            if (logger::cond_log(logger::INFO)) {
                assert_time += ((std::chrono::duration<double, std::milli>) (t8 - t7)).count();
                assert_time_total += ((std::chrono::duration<double, std::milli>) (t8 - t7)).count();
            }

            assgn.new_decision_level();
            assgn.assign_and_enqueue(decided);
        }
        t0_1 = std::chrono::high_resolution_clock::now();
        if (logger::cond_log(logger::INFO)) {
            all_time += ((std::chrono::duration<double, std::milli>) (t0_1 - t0_0)).count();
            all_time_total += ((std::chrono::duration<double, std::milli>) (t0_1 - t0_0)).count();
        }
    }
}

void solver::do_stats()  {
    if (logger::cond_log(logger::ENHANCE) && (int)conf_no % 1000 == 0 && conf_no != 0) {
        logger::log(logger::ENHANCE, "##########################");
        logger::log(logger::ENHANCE, "Propagation time: " + to_string(prop_time/1000) + "s");
        logger::log(logger::ENHANCE, "Reason-calculation time: " + to_string(reason_time/1000) + "s");
        logger::log(logger::ENHANCE, "Variable heuristic time: " + to_string(assert_time/1000) + "s");
        logger::log(logger::ENHANCE, "Remaining time: " + to_string((all_time - assert_time - prop_time - reason_time)/1000) + "s");
        logger::log(logger::ENHANCE, "No. of conflicts: " + to_string(conf_no));
        logger::log(logger::ENHANCE, "No. of learnt clauses: " + to_string(learnt_no));

        reset_times();
    }
}

void solver::do_total_stats()  {
    if (logger::cond_log(logger::ENHANCE) && (int)conf_no % 1000 == 0 && conf_no != 0) {
        logger::log(logger::ENHANCE, "##########################");
        logger::log(logger::ENHANCE, "Propagation time: " + to_string(prop_time_total/1000) + "s");
        logger::log(logger::ENHANCE, "Reason-calculation time: " + to_string(reason_time_total/1000) + "s");
        logger::log(logger::ENHANCE, "Variable heuristic time: " + to_string(assert_time_total/1000) + "s");
        logger::log(logger::ENHANCE, "Remaining time: " + to_string((all_time_total - assert_time_total - prop_time_total - reason_time_total)/1000) + "s");
        logger::log(logger::ENHANCE, "No. of conflicts: " + to_string(conf_no));
        logger::log(logger::ENHANCE, "No. of learnt clauses: " + to_string(learnt_no));
    }
}

void solver::reset_times() {
    prop_time = 0;
    reason_time = 0;
    assert_time = 0;
    all_time = 0;
}

bool solver::allAssigned() {
    for (int i = 1; i <= assgn.get_var_num(); i++) {
        if (assgn.get_assignment(i) == sat_bool::Undef) {
            return false;
        }
    }
    return true;
}

int solver::calc_reason(clause *conflict, clause *learnt, lit *asserting) {
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
            int var = (int) new_expansion.get_var();
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
        } while (!lits_seen[expansion.get_var()]);
        counter--;
    } while (counter > 0);

    *asserting = expansion.neg_copy();
    learnt->add_lit(expansion.neg_copy());

    return max_level;
}

void solver::set_state(sat_bool st) {
    state = st;
}


