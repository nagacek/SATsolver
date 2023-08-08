//
// Created by natalia on 31.05.23.
//
#include "clause.h"
#include "exceptions/sat_exception.h"
#include <vector>
#include<deque>
#include <memory>

#ifndef SATSOLVER_CNF_H
#define SATSOLVER_CNF_H

class cnf {
private:
    std::vector<shared_ptr<clause>> clauses;
    std::vector<shared_ptr<clause>> learnt_clauses;
    int clause_num;
    int fill_num;
public:
    cnf() {
        clauses = std::vector<shared_ptr<clause>>();
        learnt_clauses = std::vector<shared_ptr<clause>>();
        clause_num = -1;
    }

    weak_ptr<clause> add_clause(const std::vector<lit> & lits);

    weak_ptr<clause> add_learnt_clause(const std::vector<lit> & lits);

    void set_clause_num(int num);

    void reverse_last_learnt();

    void reverse_last();

    int occurrences(int var);

    void init_watches(watch_list *twoatch);
};


#endif //SATSOLVER_CNF_H
