//
// Created by natalia on 31.05.23.
//

#include <unordered_map>
#include <map>
#include <stack>
#include "solver.h"
#include "graph.h"
#include "occ_list.h"

bool solver::solve() {
    sat_bool status = sat_bool::Undef;
    double learnts = (long)(cnf_val.get_clause_num() / 4);
    double conf = 100;
    status = init((long)learnts);
    if (status == sat_bool::Undef) {
        status = preprocess();
    }
    logger::log(logger::INFO, "Solving");
    while (status == sat_bool::Undef) {
        status = try_solve((int)learnts, (int)conf);
        learnts *= 1.1;
        conf *= 1.5;
    }
    return status == sat_bool::True;
}

sat_bool solver::try_solve(int max_learnts, int max_conflicts) {
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
                conf_no_total++;
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

            assgn.undo_until(backtrack_level, &prio);
            weak_ptr<clause> learnt_clause = cnf_val.add_learnt_clause(learnt_lits, &prio);
            sat_bool value = learnt_clause.lock()->init_learnt(asserting, &assgn, &prio, &twoatch);

            if (value != sat_bool::Undef) {
                if (value == sat_bool::False) {
                    return sat_bool::False;
                }
                cnf_val.reverse_last_learnt();
            } else if (logger::cond_log(logger::INFO)){
                learnt_no++;
                logger::log(logger::DEBUG_VERBOSE, "Learnt reason added is " + learnt_clause.lock()->to_string(true));
            }


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
                logger::log(logger::ENHANCE, "Restarting");
                conf_no = 0;
                assgn.undo_until(0, &prio);
                restarts++;
                return sat_bool::Undef;
            }

            auto t7_1 = std::chrono::high_resolution_clock::now();
            if (cnf_val.get_learnt_num() - assgn.get_assgn_num() > max_learnts) {
                cnf_val.prune_clauses(&prio, &twoatch);
            }
            auto t8_1 = std::chrono::high_resolution_clock::now();
            if (logger::cond_log(logger::INFO)) {
                prune_time += ((std::chrono::duration<double, std::milli>) (t8_1 - t7_1)).count();
                prune_time_total += ((std::chrono::duration<double, std::milli>) (t8_1 - t7_1)).count();
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

sat_bool solver::init(long learnts) {
    auto t0_0 = std::chrono::high_resolution_clock::now();
    logger::log(logger::INFO, "Initializing");
    if (cnf_val.init(&assgn, learnts) == sat_bool::False) {
        return sat_bool::False;
    }
    prio.init(assgn.get_var_num());
    prio.occurrence_count(&cnf_val);
    auto t0_1 = std::chrono::high_resolution_clock::now();
    logger::log(logger::ENHANCE, "Initializing finished ("+ to_string(((std::chrono::duration<double, std::milli>)(t0_1 - t0_0)).count()) + "ms)");

    if (logger::cond_log(logger::INFO)) {
        all_time += ((std::chrono::duration<double, std::milli>) (t0_1 - t0_0)).count();
        all_time_total += ((std::chrono::duration<double, std::milli>) (t0_1 - t0_0)).count();
    }
    return sat_bool::Undef;
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

    do {
        auto conflict_lock = conflict.lock();
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
            assgn.undo_last(&prio);
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

// ######## preprocessing ########
// TODO: Iterations/Preprocessing order and Unhide, stats
sat_bool solver::preprocess() {
    auto t2 = std::chrono::high_resolution_clock::now();

    // occurrence list for preprocessing
    occ_list occ_list{&cnf_val};
    occ_list.init(assgn.get_var_num());
    cnf_val.init_all_watches(&occ_list);

    vector<weak_ptr<clause>> binary{};
    cnf_val.find_binary_clauses(binary);
    graph g;
    for (auto &b : binary) {
        if (!b.expired()) {
            g.add_clause(b);
        }
    }

    // pure literal
    weak_ptr<clause> conflict = assgn.propagate(&occ_list);
    if (!conflict.expired()) {
        return sat_bool::False;
    }
    occ_list.propagate_pure_literal(assgn);
    conflict = assgn.propagate(&occ_list);
    if (!conflict.expired()) {
        return sat_bool::False;
    }

    // update binary list
    auto new_binaries = occ_list.poll_new_binary();
    for (auto &b : new_binaries) {
        if (!b.expired()) {
            g.add_clause(b);
        }
    }

    logger::log(logger::ENHANCE, "No. of binary clauses: " + to_string(binary.size()));

    // Propagation for simplification =>
    // As there are no decisions, propagations do not have to be reversible.
    // This results in permanent changes in the formula.
    // If there are conflicts, they are on root level and the formula is unsat.
    conflict = assgn.propagate(&occ_list);
    if (!conflict.expired()) {
        return sat_bool::False;
    }

    new_binaries = occ_list.poll_new_binary();
    for (auto &b : new_binaries) {
        if (!b.expired()) {
            g.add_clause(b);
        }
    }
    set<lit> roots = g.find_roots();
    // unhiding here prob.
    vector<vector<lit>> sccs {}; //= g.find_sccs();
    logger::log(logger::ENHANCE, "No. of SCCs: " + to_string(sccs.size()));
    for (auto &vec : sccs) {
        logger::log(logger::ENHANCE, "SCC: " + logger::vec_to_string(vec));
        sat_bool values = sat_bool::Undef;
        lit repr{0, false};
        for (auto &l : vec) {
            sat_bool curr_val = assgn.apply(l);
            if (values == sat_bool::Undef) {
                values = curr_val;
                repr = l;
            } else if (curr_val != sat_bool::Undef && values != curr_val) {
                logger::log(logger::DEBUG, "Equivalent Literals did not have the same assigned value");
                return sat_bool::False;
            }
        }
        for (auto l : vec) {
            if (!assgn.has_representant(l)) {
                assgn.set_representant(l, repr);
                if (l == repr) {
                    continue;
                }
                if (occ_list.substitute(l, repr, &assgn) == sat_bool::False) {
                    return sat_bool::False;
                }
            }
        }
    }

    conflict = assgn.propagate(&occ_list);
    if (!conflict.expired()) {
        return sat_bool::False;
    }

    // ######## stats ########
    auto t3 = std::chrono::high_resolution_clock::now();
    logger::log(logger::INFO,"Preprocessing finished (" + to_string(((std::chrono::duration<double, std::milli>) (t3 - t2)).count()) + "ms)");

    // Init two-watched lists only after all permanent changes are made
    cnf_val.init_watches(&twoatch);
    return sat_bool::Undef;
}

// ######## stats #########

void solver::reset_times() {
    prop_time = 0;
    reason_time = 0;
    assert_time = 0;
    all_time = 0;
    prune_time = 0;
}

void solver::do_stats()  {
    if (logger::cond_log(logger::ENHANCE) && (int)conf_no % 1000 == 0 && conf_no != 0) {
        logger::log(logger::ENHANCE, "##########################");
        logger::log(logger::ENHANCE, "Propagation time: " + to_string(prop_time/1000) + "s");
        logger::log(logger::ENHANCE, "Reason-calculation time: " + to_string(reason_time/1000) + "s");
        logger::log(logger::ENHANCE, "Variable heuristic time: " + to_string(assert_time/1000) + "s");
        logger::log(logger::ENHANCE, "Pruning time: " + to_string(prune_time/1000) + "s");
        logger::log(logger::ENHANCE, "Remaining time: " + to_string((all_time - assert_time - prop_time - reason_time - prune_time)/1000) + "s");
        logger::log(logger::ENHANCE, "No. of conflicts: " + to_string(conf_no));
        logger::log(logger::ENHANCE, "No. of learnt clauses: " + to_string(cnf_val.get_learnt_num()));

        reset_times();
    }
}

void solver::do_total_stats()  {
    if (logger::cond_log(logger::INFO)) {
        logger::log(logger::INFO, "##########################");
        logger::log(logger::INFO, "Propagation time: " + to_string(prop_time_total/1000) + "s");
        logger::log(logger::INFO, "Reason-calculation time: " + to_string(reason_time_total/1000) + "s");
        logger::log(logger::INFO, "Variable heuristic time: " + to_string(assert_time_total/1000) + "s");
        logger::log(logger::INFO, "Pruning time: " + to_string(prune_time_total/1000) + "s");
        logger::log(logger::INFO, "Remaining time: " + to_string((all_time_total - assert_time_total - prop_time_total - reason_time_total - prune_time_total)/1000) + "s");
        logger::log(logger::INFO, "No. of conflicts: " + to_string(conf_no_total));
        logger::log(logger::INFO, "No. of learnt clauses: " + to_string(learnt_no));
        logger::log(logger::INFO, "No. of restarts: " + to_string(restarts));
    }
}