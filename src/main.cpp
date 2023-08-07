#include <iostream>
#include "parser.h"
#include "solver.h"
#include <chrono>

int main(int argc, char **argv) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    solver solver(1.0, 1.05);

    auto t1 = std::chrono::high_resolution_clock::now();
    std::string cnf_file = parser::parse(argc, argv, solver);
    auto t2 = std::chrono::high_resolution_clock::now();

    if (logger::cond_log(logger::ENHANCE)) {
        logger::log(logger::ENHANCE,
                    "Parsing finished (" + to_string(((duration<double, std::milli>) (t2 - t1)).count()) + "ms)");
    } else {
        logger::log(logger::INFO, "Parsing finished");
    }

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

    solver.do_total_stats();
}
