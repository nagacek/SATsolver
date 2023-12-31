//
// Created by natalia on 31.05.23.
//
#include <memory>
#include <vector>
#include <set>
#include "lit.h"

#ifndef SATSOLVER_CLAUSE_H
#define SATSOLVER_CLAUSE_H

class watch_list;
class assignment;
class priority;
class clause : public std::enable_shared_from_this<clause> {
private:
    int watch1;
    int watch2;
    bool learnt;
    std::vector<lit> lits;
public:
    explicit clause(std::vector<lit> lits) {
        watch1 = -1;
        watch2 = -1;
        learnt =  false;
        this->lits = std::vector<lit>(lits);
    }

    //void add_lit(lit lit);

    bool propagate(lit lit, watch_list* twoatch, assignment* assgn);

    bool swap_watch2(watch_list *twoatch, assignment *assgn);

    void init_watch(watch_list *twoatch);

    void init_occurrences(watch_list * all_watches);

    sat_bool simplify(lit lit, watch_list* twoatch, assignment* assgn);

    sat_bool init_learnt(lit watch, assignment* assgn, priority *prio, watch_list *twoatch);

    void calc_reason(lit of, vector<lit> *reason);

    sat_bool init(assignment *assgn);

    void occurrences(vector<int> &vector);

    std::string to_string(bool show_watches);

    void cancel_watches(watch_list *twoatch);

    bool is_learnt();

    int get_size();

    lit get_binary(bool first);

    set<lit> get_lits();
};
#include "assignment.h"
#include "priority.h"
#include "watch_list.h"

#endif //SATSOLVER_CLAUSE_H
