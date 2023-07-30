//
// Created by natalia on 31.05.23.
//

#ifndef SATSOLVER_ASSIGNMENT_H
#define SATSOLVER_ASSIGNMENT_H


#include "sat_bool.h"
#include "exceptions/sat_exception.h"
#include "lit.h"
#include <vector>

class assignment {
private:
    std::vector<sat_bool> assgn;
    int var_num;
public:
    assignment() {
        assgn = std::vector<sat_bool>();
    }
    void init(int var) {
        assgn[var] = sat_bool::Undef;
    }
    void assign(lit* &mk_true) {
        assgn.at(mk_true->get_var()) = !assgn.at(mk_true->get_var());
    }

    sat_bool apply(lit* &lit) {
        return lit->is_neg() ? !assgn.at(lit->get_var()) : assgn.at(lit->get_var());
    }

    sat_bool get_assignment(int var) {
        return assgn.at(var);
    }

    void set_var_num(int num) {
        if (num <= 0)
            throw invalid_arg_exception(std::string("negative amount of variables"));
        var_num = num;
        assgn.resize(num + 1);
    }

};


#endif //SATSOLVER_ASSIGNMENT_H
