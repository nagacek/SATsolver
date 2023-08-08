//
// Created by natalia on 31.05.23.
//

#include "solver.h"

bool solver::solve() {
    init();
    double learnts = (int)(cnf_val.get_clause_num() / 4);
    double conf = (int)(cnf_val.get_clause_num() / 3);
    sat_bool status = sat_bool::Undef;
    while (status == sat_bool::Undef) {
        status = try_solve((int)learnts, (int)conf);
        learnts *= 1.1;
        conf *= 1.5;
    }
    return status == sat_bool::True;
}

sat_bool solver::try_solve(int max_learnts, int max_conflicts) {
    logger::log(logger::INFO, "Solving");
    if (state == sat_bool::False) {
        return sat_bool::False;
    }

    while (true) {
        auto t0_0 = std::chrono::high_resolution_clock::now();
        auto t1 = std::chrono::high_resolution_clock::now();
        weak_ptr<clause> conflict = assgn.propagate(&twoatch);
        auto t2 = std::chrono::high_resolution_clock::now();
        if (logger::cond_log(logger::INFO)) {
            prop_time += ((std::chrono::duration<double, std::milli>) (t2 - t1)).count();
            prop_time_total += ((std::chrono::duration<double, std::milli>) (t2 - t1)).count();
        }

        if (!conflict.expired()) {
            if (logger::cond_log(logger::INFO)) {
                conf_no++;
            }
            if (assgn.get_level() == 0) {
                return sat_bool::False;
            }

            std::vector<lit> learnt_lits;
            lit asserting = {0, false};

            auto t3 = std::chrono::high_resolution_clock::now();
            int backtrack_level = calc_reason(conflict, learnt_lits, &asserting);
            auto t4 = std::chrono::high_resolution_clock::now();
            if (logger::cond_log(logger::INFO)) {
                reason_time += ((std::chrono::duration<double, std::milli>) (t4 - t3)).count();
                reason_time_total += ((std::chrono::duration<double, std::milli>) (t4 - t3)).count();
            }

            assgn.undo_until(backtrack_level);
            weak_ptr<clause> learnt_clause = cnf_val.add_learnt_clause(learnt_lits, &prio);
            sat_bool value = learnt_clause.lock()->init_learnt(asserting, &assgn, &prio, &twoatch);

            if (value != sat_bool::Undef) {
                if (value == sat_bool::False) {
                    return sat_bool::False;
                }
                cnf_val.reverse_last_learnt();
            } else if (logger::cond_log(logger::INFO)){
                learnt_no++;
            }

            logger::log(logger::DEBUG_VERBOSE, "Learnt reason added is " + learnt_clause.lock()->to_string(true));

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
            if (conf_no > max_conflicts) {
                conf_no = 0;
                assgn.undo_until(0);
                return sat_bool::Undef;
            }

            if (cnf_val.get_learnt_num() - assgn.get_assgn_num() > max_learnts) {
                cnf_val.prune_clauses(&prio, &twoatch);
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
        auto t0_1 = std::chrono::high_resolution_clock::now();
        if (logger::cond_log(logger::INFO)) {
            all_time += ((std::chrono::duration<double, std::milli>) (t0_1 - t0_0)).count();
            all_time_total += ((std::chrono::duration<double, std::milli>) (t0_1 - t0_0)).count();
        }
    }
}

void solver::init() {
    auto t0_0 = std::chrono::high_resolution_clock::now();
    logger::log(logger::INFO, "Initializing");
    cnf_val.init_watches(&twoatch);
    prio.init(assgn.get_var_num());
    auto t0_1 = std::chrono::high_resolution_clock::now();

    if (logger::cond_log(logger::INFO)) {
        all_time += ((std::chrono::duration<double, std::milli>) (t0_1 - t0_0)).count();
        all_time_total += ((std::chrono::duration<double, std::milli>) (t0_1 - t0_0)).count();
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

int solver::calc_reason(weak_ptr<clause> conflict, vector<lit> & learnt, lit *asserting) {
    lit expansion = {0, false};
    int counter = 0;
    int max_level = 0;

    vector<bool> lits_seen = vector<bool>(assgn.get_var_num() + 1);
    std::fill(lits_seen.begin(), lits_seen.end(), false);

    vector<lit> inter_reason = vector<lit>();
    auto conflict_lock = conflict.lock();

    do {
        if (conflict_lock->is_learnt()){
            prio.cla_enhance(cnf_val.find_learnt(conflict));
        }

        inter_reason.clear();
        conflict_lock->calc_reason(expansion, &inter_reason);

        for (int i = 0; i < inter_reason.size(); i++) {
            lit new_expansion = inter_reason[i];
            int var = (int) new_expansion.get_var();
            if (!lits_seen[var]) {
                lits_seen[var] = true;
                int exp_level = assgn.get_level(new_expansion);
                if (exp_level == assgn.get_level()) {
                    counter++;
                } else if (exp_level > 0) {
                    learnt.push_back(new_expansion.neg_copy());
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
    learnt.push_back(expansion.neg_copy());

    return max_level;
}

void solver::set_state(sat_bool st) {
    state = st;
}

// ######## stats #########

void solver::reset_times() {
    prop_time = 0;
    reason_time = 0;
    assert_time = 0;
    all_time = 0;
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
    if (logger::cond_log(logger::ENHANCE)) {
        logger::log(logger::ENHANCE, "##########################");
        logger::log(logger::ENHANCE, "Propagation time: " + to_string(prop_time_total/1000) + "s");
        logger::log(logger::ENHANCE, "Reason-calculation time: " + to_string(reason_time_total/1000) + "s");
        logger::log(logger::ENHANCE, "Variable heuristic time: " + to_string(assert_time_total/1000) + "s");
        logger::log(logger::ENHANCE, "Remaining time: " + to_string((all_time_total - assert_time_total - prop_time_total - reason_time_total)/1000) + "s");
        logger::log(logger::ENHANCE, "No. of conflicts: " + to_string(conf_no));
        logger::log(logger::ENHANCE, "No. of learnt clauses: " + to_string(learnt_no));
    }
}



