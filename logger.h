//
// Created by natalia on 30.07.23.
//

#ifndef SATSOLVER_LOGGER_H
#define SATSOLVER_LOGGER_H

#include <string>
#include <fstream>
#include <cstdio>

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
        DEBUG_VERBOSE
    };
    const type LOG_LEVEL = type::DEBUG_VERBOSE;

    const std::string message_types[] =
            {
                    "FINISHED",
                    "ERROR",
                    "INFO",
                    "WARNING",
                    "DEBUG",
                    "ENHANCE",
                    "DEBUG_VERBOSE"
            };


    void log(type type_num, const std::string &msg);

    void log_stdout(type type_num, const std::string &msg);
};


#endif //SATSOLVER_LOGGER_H
