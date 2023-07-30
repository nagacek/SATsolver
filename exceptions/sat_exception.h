//
// Created by natalia on 31.05.23.
//
#include <string>
#include <cstring>
#include "exception"

#ifndef SATSOLVER_SAT_EXCEPTION_H
#define SATSOLVER_SAT_EXCEPTION_H


    using namespace std;

    class sat_exception : std::exception {
        char* what() {
            return (char*)"The solver cannot handle this.";
        }
    };

    class parser_exception : sat_exception {
        char* what() {
            return (char*)"The parsing went wrong.";
        }
    };

    class invalid_file_exception : parser_exception {
        char* what() {
            return (char*) "Not a file";
        }
    };

    class invalid_arg_exception : parser_exception {
        std::string inv;

    public:
        invalid_arg_exception(std::string arg) : inv(std::string("Invalid argument: ") + arg) {}

        char* what() {
            return strdup(inv.c_str());
        }
    };

    class invalid_format_exception : parser_exception {
        std::string inv;

    public:
        invalid_format_exception(std::string arg) : inv(std::string("Invalid argument: ") + arg) {}

        char* what() {
            return strdup(inv.c_str());
        }
    };


#endif //SATSOLVER_SAT_EXCEPTION_H
