//
// Created by natalia on 31.05.23.
//
#include "clause.h"
#include "exceptions/sat_exception.h"
#include <vector>
#include<deque>

#ifndef SATSOLVER_CNF_H
#define SATSOLVER_CNF_H

class cnf {
private:
    std::vector<clause> clauses;
    std::deque<clause> learnt_clauses;
    int clause_num;
public:
    cnf() {
        clauses = std::vector<clause>();
        learnt_clauses = std::deque<clause>();
        clause_num = -1;
    }

    clause* add_clause();

    clause* add_learnt_clause();

    void set_clause_num(int num);

    void reverse_last_learnt();

    void reverse_last();

    int occurrences(int var);

    void init_watches(watch_list *twoatch);
};


#endif //SATSOLVER_CNF_H
