#include <iostream>
#include "parser.h"
#include "solver.h"

int main(int argc,  char **argv) {
    solver solver(1.0f, 0.95f);

    std::string cnf_file = parser::parse(argc, argv, solver);

    sat_bool result = solver.solve();
    if (result == sat_bool::True) {
        log_stdout(logger::type::FINISHED, "SATISFIABLE");
        parser::unparse(solver.get_assignment(), cnf_file);
    } else if (result == sat_bool::False) {
        log_stdout(logger::type::FINISHED, "UNSATISFIABLE");
    } else {
        log(logger::type::ERROR, "No result obtained when finished.");
        exit(-1);
    }
}
