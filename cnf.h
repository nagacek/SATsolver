//
// Created by natalia on 31.05.23.
//
#include "clause.h"
#include "exceptions/sat_exception.h"
#include <vector>

#ifndef SATSOLVER_CNF_H
#define SATSOLVER_CNF_H


class cnf {
private:
    std::vector<clause> clauses;
    int clause_num;
public:
    cnf() {
        clauses = std::vector<clause>();
        clause_num = -1;
    }
    clause* add_clause() {
        auto new_clause = clauses.insert(clauses.end(), clause());
        return new_clause.base();
    }

    void set_clause_num(int num) {
        if (num <= 0)
            throw invalid_arg_exception(std::string("negative number of clauses"));
        clause_num = num;
    }

};


#endif //SATSOLVER_CNF_H
