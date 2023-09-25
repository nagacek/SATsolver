//
// Created by natalia on 02.08.23.
//

#include "priority.h"
#include "cnf.h"

void priority::init(int var_num) {
    prio.resize(var_num + 1);
    occurrences.resize(var_num + 1);
}

lit priority::decide(assignment *assgn, cnf *cnf) {
    int decided = 0;
    double decided_val = -1;
    for (int i = 1; i < prio.size(); i++) {
        double current_val = prio[i];
        if (current_val > decided_val && assgn->get_assignment(i) == sat_bool::Undef) {
            decided = i;
            decided_val = current_val;
        }
    }
    if (decided == 0) {
        log(logger::type::ERROR,
            "Decision procedure found no variable with highest priority even though there is no satisfying assignment.");
        exit(-1);
    }

    return {assgn->get_representant(decided), occurrences[decided] < 0};
}

void priority::enhance(int var) {
    prio[var] += variable_enhance;
    if (prio[var] > 1e100) {
        for (auto &p : prio) {
            p *= 1e-100;
        }
        variable_enhance *= 1e-99;
    }
}

void priority::cla_enhance(int num) {
    if (num < 0) {
        logger::log(logger::ERROR, "Invalid learnt clause (could not be found)");
    }
    clause_prio[num] += clause_enhance;
    if (clause_prio[num] > 1e100) {
        for (auto &p : clause_prio) {
            p *= 1e-100;
        }
        clause_enhance *= 1e-99;
    }
}

void priority::update() {
    variable_enhance *= variable_diminish;
    clause_enhance *= clause_diminish;
}

double priority::calc_median() {
    vector<double> median_vec(clause_prio);
    double pivot_val;
    if (old_pivot > -1) {
        pivot_val = old_pivot;
    } else {
        int pivot = (int)(std::rand()) % median_vec.size();
        pivot_val = median_vec[pivot];
    }
    int l = 0;
    int r = (int)median_vec.size() - 1;
    int target = (int)(median_vec.size()/2);

    while(l < r) {
        int i = l;
        int j = r;
        split(median_vec, i, j, pivot_val);
        if (j <= target)
            l = i;
        if (i >= target)
            r = j;
        if (abs(l - target) <= target/10)
            return median_vec[l];
        if (abs(r - target) <= target/10)
            return median_vec[r];
        int pivot = ((int)(std::rand()) % (r - l)) + l;
        pivot_val = median_vec[pivot];
    }
    return median_vec[r];
}

void priority::split(vector<double> &median_vec, int &i, int &j, double pivot_val) {
    do {
        while (median_vec[i] <= pivot_val && i < j)
            i++;
        while (median_vec[j] > pivot_val)
            j--;
        double temp = median_vec[i];
        median_vec[i] = median_vec[j];
        median_vec[j] = temp;
    } while(i < j);
}

double priority::get_cla_prio(int num) {
    return clause_prio[num];
}

void priority::delete_cla(int num) {
    clause_prio.erase(clause_prio.begin() + num);
}

double priority::get_cla_thresh() {
    return clause_enhance / (int)clause_prio.size();
}

void priority::new_cla() {
    clause_prio.push_back(clause_enhance);
}

void priority::occurrence_count(cnf * cnf_val) {
    cnf_val->occurrences(occurrences);
}

void priority::save_phase(lit lit) {
    occurrences[lit.get_var()] = lit.is_neg() ? -1 : 1;
}
