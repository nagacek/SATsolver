//
// Created by natalia on 31.05.23.
//
#include <vector>
#include "lit.h"

#ifndef SATSOLVER_CLAUSE_H
#define SATSOLVER_CLAUSE_H


class clause {
private:
    std::vector<lit> lits;
public:
    clause() {
        lits = std::vector<lit>();
    }
    void add_lit(lit lit) {
        lits.insert(lits.end(), lit);
    }
};


#endif //SATSOLVER_CLAUSE_H
