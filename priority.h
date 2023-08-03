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

public:
    priority() {
        variable_enhance = 0;
        variable_diminish = 0;
        prio = std::vector<double>();
    }
    explicit priority(double plus, double minus) {
        variable_enhance = plus;
        variable_diminish = minus;
        prio = std::vector<double>();
    }

    void init(int var_num);

    lit decide(assignment *assgn, cnf *cnf);

    void enhance(int var);

    void update();
};

#include "assignment.h"
#include "cnf.h"

#endif //SATSOLVER_PRIORITY_H
