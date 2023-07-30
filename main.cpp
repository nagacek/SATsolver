#include <iostream>
#include "parser.h"
#include "solver.h"
int main(int argc,  char **argv) {
    solver solver;
    parser::parse(argc, argv, solver);


}
