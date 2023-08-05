//
// Created by natalia on 30.07.23.
//

#ifndef SATSOLVER_LOGGER_H
#define SATSOLVER_LOGGER_H

#include <string>
#include <fstream>
#include <cstdio>
#include "lit.h"

namespace logger {
    const std::string file = "stdout";
    const bool use_file = false;

    enum type {
        FINISHED,
        ERROR,
        INFO,
        WARNING,
        DEBUG,
        ENHANCE,
        DEBUG_VERBOSE,
        DEBUG_VERY_VERBOSE
    };
    const type LOG_LEVEL = type::INFO;

    const std::string message_types[] =
            {
                    "FINISHED",
                    "ERROR",
                    "INFO",
                    "WARNING",
                    "DEBUG",
                    "ENHANCE",
                    "DEBUG_VERBOSE",
                    "DEBUG_VERY_VERBOSE"
            };


    void log(type type_num, const std::string &msg);

    void log_stdout(type type_num, const std::string &msg);

    bool cond_log(type type_num);
};


#endif //SATSOLVER_LOGGER_H
