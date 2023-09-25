//
// Created by natalia on 22.09.23.
//

#include "occ_list.h"

weak_ptr<clause> occ_list::propagate(lit lit, assignment *assgn) {
    logger::log(logger::DEBUG_VERBOSE, lit.to_string() + " is propagated in occurrence-list");

    vector<weak_ptr<clause>> temp_list{};
    std::swap(list[lit.get_id()], temp_list);
    //temp_list = std::move(list[lit.get_id()]); //??
    //temp_list = list[lit.get_id()];
    vector<weak_ptr<clause>> ntemp_list{};
    std::swap(list[lit.get_nid()], ntemp_list);
    //undo(lit);
    for (auto it = temp_list.begin(); it != temp_list.end(); ) {
        if (!(*it).expired()) {
            auto locked = (*it).lock();
            sat_bool result = locked->simplify(lit, this, assgn);
            locked.reset();
            if (result == sat_bool::False) {
                return *it;
            } else if (result == sat_bool::Undef) {
                cnf_val->delete_clause(*it);
            }
        }
        it = temp_list.erase(it);
    }
    for (auto it = ntemp_list.begin(); it != ntemp_list.end(); ) {
        if (!(*it).expired()) {
            auto locked = (*it).lock();
            sat_bool result = locked->simplify(lit.neg_copy(), this, assgn);
            locked.reset();
            if (result == sat_bool::False) {
                return *it;
            } else if (result == sat_bool::Undef) {
                cnf_val->delete_clause(*it);
            }
        }
        it = ntemp_list.erase(it);
    }
    return {};
}

void occ_list::propagate_pure_literal(assignment& assgn) {
    for (int i = 1; i <= assgn.get_var_num(); i++) {
        lit pos{i, false};
        auto pos_list = list[pos.get_id()];

        // in case that there are invalidated clause pointers, these have to be removed first
        for (auto it = pos_list.begin(); it != pos_list.end();) {
            if ((*it).expired()){
                it = pos_list.erase(it);
            } else {
                it++;
            }
        }
        auto neg_list = list[pos.get_nid()];
        for (auto it = neg_list.begin(); it != neg_list.end();) {
            if ((*it).expired()){
                it = neg_list.erase(it);
            } else {
                it++;
            }
        }
        if (pos_list.empty() && assgn.apply(pos.neg_copy()) == sat_bool::Undef) {
            assgn.assign_and_enqueue(pos.neg_copy());
        } else if (neg_list.empty() && assgn.apply(pos) == sat_bool::Undef) {
            assgn.assign_and_enqueue(pos);
        }
    }
}

vector<weak_ptr<clause>> occ_list::poll_new_binary() {
    vector<weak_ptr<clause>> ret_val{binary};
    binary.clear();
    return ret_val;
}

sat_bool occ_list::substitute(lit eq, lit repr, assignment * assgn) {
    logger::log(logger::DEBUG, "Substituting " + eq.to_string() + " with " + repr.to_string());
    vector<weak_ptr<clause>> eq_list{};
    std::swap(list[eq.get_id()], eq_list);
    for (auto & it : eq_list) {
        if (!it.expired()) {
            auto shared_cl = it.lock();
            cnf_val->delete_clause(shared_cl);
            auto lits = shared_cl->get_lits();
            vector<lit> lit_vec{lits.begin(), lits.end()};
            shared_cl.reset();
            std::replace(lit_vec.begin(), lit_vec.end(), eq, repr);
            auto new_cl = cnf_val->add_clause(lit_vec);
            shared_cl = new_cl.lock();
            sat_bool result = shared_cl->init(assgn);
            if (result != sat_bool::Undef) {
                if (result == sat_bool::False) {
                    return sat_bool::False;
                }
                cnf_val->delete_clause(shared_cl);
            }
            shared_cl->init_occurrences(this);
        }
    }
    vector<weak_ptr<clause>> neq_list{};
    std::swap(list[eq.get_nid()], neq_list);
    for (auto & it : neq_list) {
        if (!it.expired()) {
            auto shared_cl = it.lock();
            cnf_val->delete_clause(shared_cl);
            auto lits = shared_cl->get_lits();
            vector<lit> lit_vec{lits.begin(), lits.end()};
            shared_cl.reset();
            std::replace(lit_vec.begin(), lit_vec.end(), eq.neg_copy(), repr.neg_copy());
            auto new_cl = cnf_val->add_clause(lit_vec);
            shared_cl = new_cl.lock();
            sat_bool result = shared_cl->init(assgn);
            if (result != sat_bool::Undef) {
                if (result == sat_bool::False) {
                    return sat_bool::False;
                }
                cnf_val->delete_clause(shared_cl);
            }
            shared_cl->init_occurrences(this);
        }
    }
    return sat_bool::Undef;
}

void occ_list::notify(weak_ptr<clause> clause) {
    binary.emplace_back(clause);
}

