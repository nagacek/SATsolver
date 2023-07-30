//
// Created by natalia on 31.05.23.
//

#include "solver.h"

#ifndef SATSOLVER_PARSER_H
#define SATSOLVER_PARSER_H
namespace parser {
    std::string parse(int argc, char** argv, solver& s);

    void unparse(assignment *assgn, const string& cnf_file);
}
#endif //SATSOLVER_PARSER_H
