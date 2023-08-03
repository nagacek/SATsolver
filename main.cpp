#include <iostream>
#include "parser.h"
#include "solver.h"

int main(int argc,  char **argv) {
    solver solver(1.0, 1.05);

    logger::log(logger::INFO, "Parsing");
    std::string cnf_file = parser::parse(argc, argv, solver);
    logger::log(logger::INFO, "Parsing finished");

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
