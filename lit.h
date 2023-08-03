//
// Created by natalia on 31.05.23.
//
#include "sat_bool.h"
#include "exceptions/sat_exception.h"
#ifndef SATSOLVER_LIT_H
#define SATSOLVER_LIT_H


class lit {
private:
    int var;
    bool is_negative;
    int id;
    int nid;
public:
    lit(int var, bool neg) {
        if (var == 0 && neg || var < 0) {
            throw invalid_arg_exception(std::string("lit variable cannot have index 0 and be negative"));
        }
        this->var = var;
        is_negative = neg;
        id = 2 * var - (is_negative ? 1 : 0);
        nid = id == 0 ? 0 : 2 * var - (is_negative ? 0 : 1);

    }

    explicit lit(int variable) {
        if (variable == 0) {
            throw invalid_arg_exception(std::string("lit variable cannot have index 0"));
        }
        this->var = abs(variable);
        is_negative = variable < 0;
        id = 2 * var - (is_negative ? 1 : 0);
        nid = id == 0 ? 0 : 2 * var - (is_negative ? 0 : 1);
    }
    unsigned get_id() { return id; }
    unsigned get_nid() { return nid; }
    unsigned get_var() { return var; }
    bool is_neg() { return is_negative; }
    lit copy() { return {var, is_negative}; }
    lit neg_copy() { return {var, !is_negative}; }
    bool operator==(lit b){ return get_id() == b.get_id(); }
};


#endif //SATSOLVER_LIT_H
