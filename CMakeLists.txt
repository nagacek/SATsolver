cmake_minimum_required(VERSION 3.20)
project(SATsolver)

set(CMAKE_CXX_STANDARD 17)

add_executable(SATsolver parser.cpp parser.h main.cpp exceptions/sat_exception.h solver.cpp solver.h cnf.cpp cnf.h clause.h lit.h sat_bool.h assignment.h watch_list.cpp watch_list.h
        logger.cpp
        logger.h)
