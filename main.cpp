#include <iostream>
#include "parser.h"
#include "solver.h"

int main(int argc,  char **argv) {
    solver solver;
    parser::parse(argc, argv, solver);

    sat_bool result = solver.solve();
    if (result == sat_bool::True) {
        logger::log
    }
}
