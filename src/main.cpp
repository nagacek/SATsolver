#include <iostream>
#include "parser.h"
#include "solver.h"
#include <chrono>

int main(int argc, char **argv) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = std::chrono::high_resolution_clock::now();
    solver solver(1.0, 1.05, 1.0, 1.05);

    std::string cnf_file = parser::parse(argc, argv, solver);
    auto t2 = std::chrono::high_resolution_clock::now();

    if (logger::cond_log(logger::ENHANCE)) {
        logger::log(logger::ENHANCE,
                    "Parsing finished (" + to_string(((duration<double, std::milli>) (t2 - t1)).count()) + "ms)");
    } else {
        logger::log(logger::INFO, "Parsing finished");
    }

    if (solver.solve()) {
        log_stdout(logger::type::FINISHED, "SATISFIABLE");
        parser::unparse(solver.get_assignment(), cnf_file);
    } else {
        log_stdout(logger::type::FINISHED, "UNSATISFIABLE");
    }
    auto t3 = std::chrono::high_resolution_clock::now();

    solver.do_total_stats();
    logger::log(logger::INFO, "Total time: " + to_string(((duration<double, std::milli>) (t3 - t1)/1000).count()) + "s");
}
