//
// Created by natalia on 22.09.23.
//

#ifndef SATSOLVER_OCC_LIST_H
#define SATSOLVER_OCC_LIST_H


#include "watch_list.h"

class occ_list : public watch_list {
private:
    cnf * cnf_val;
    vector<weak_ptr<clause>> binary;
public:
    explicit occ_list(cnf * cnf) {
        cnf_val = cnf;
    }

    weak_ptr<clause> propagate(lit lit, assignment *assgn) override;

    void notify(weak_ptr<clause> clause) override;

    void propagate_pure_literal(assignment& assgn);

    vector<weak_ptr<clause>> poll_new_binary();

    sat_bool substitute(lit eq, lit repr, assignment * assgn);
};


#endif //SATSOLVER_OCC_LIST_H
