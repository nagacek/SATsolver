//
// Created by natalia on 31.05.23.
//
#include <vector>
#include "lit.h"

#ifndef SATSOLVER_CLAUSE_H
#define SATSOLVER_CLAUSE_H

class watch_list;
class assignment;
class clause {
private:
    std::vector<lit> lits;
    int watch1;
    int watch2;
public:
    clause() {
        lits = std::vector<lit>();
        watch1 = -1;
        watch2 = -1;
    }

    void add_lit(lit lit);

    bool propagate(lit lit, watch_list* twoatch, assignment* assgn);

    bool swap_watch1(watch_list *twoatch, assignment *assgn);

    void init_watch(lit new_watch1, lit new_watch2);
};
#include "assignment.h"

#endif //SATSOLVER_CLAUSE_H
