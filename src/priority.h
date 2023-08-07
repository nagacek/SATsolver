//
// Created by natalia on 02.08.23.
//

#ifndef SATSOLVER_PRIORITY_H
#define SATSOLVER_PRIORITY_H


#include <vector>
#include "lit.h"

class assignment;
class cnf;
class priority {

private:
    std::vector<double> prio;
    double variable_enhance;
    double variable_diminish;

    std::vector<double> clause_prio;
    double old_pivot = -1;
    double clause_enhance;
    double clause_diminish;

public:
    priority() {
        variable_enhance = 0;
        variable_diminish = 0;
        prio = std::vector<double>();

        clause_prio = std::vector<double>();
        clause_enhance = 0;
        clause_diminish = 0;
    }
    explicit priority(double plus, double minus, double cla_plus, double cla_minus) {
        variable_enhance = plus;
        variable_diminish = minus;
        prio = std::vector<double>();

        clause_prio = std::vector<double>();
        clause_enhance = cla_plus;
        clause_diminish = cla_minus;
    }

    void init(int var_num);

    lit decide(assignment *assgn, cnf *cnf);

    void enhance(int var);

    void cla_enhance(int num);

    void new_cla();

    void update();

    double calc_median();

    double get_cla_prio(int num);

    void delete_cla(int num);

    double get_cla_thresh();

    void split(vector<double> &median_vec, int &i, int &j, double pivot_val);
};

#include "assignment.h"
#include "cnf.h"

#endif //SATSOLVER_PRIORITY_H
