cmake_minimum_required(VERSION 3.20)
project(SATsolver)

set(CMAKE_CXX_STANDARD 17)

add_executable(SATsolver src/parser.cpp src/parser.h src/main.cpp src/exceptions/sat_exception.h src/solver.cpp src/solver.h src/cnf.cpp src/cnf.h src/clause.h src/lit.h src/sat_bool.h src/assignment.h src/watch_list.cpp src/watch_list.h
        src/logger.h
        src/logger.cpp
        src/assignment.cpp
        src/clause.cpp
        src/priority.cpp
        src/priority.h
        src/graph.cpp
        src/graph.h)

