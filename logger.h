//
// Created by natalia on 30.07.23.
//

#ifndef SATSOLVER_LOGGER_H
#define SATSOLVER_LOGGER_H

#include <string>
#include <fstream>
#include <cstdio>
#include <chrono>


namespace logger {
    std::string file = "stdout";
    bool use_file = false;

    enum type {
        FINISHED,
        ERROR,
        INFO,
        WARNING,
        DEBUG,
        ENHANCE,
        DEBUG_VERBOSE
    };
    type LOG_LEVEL = type::ENHANCE;

    std::string message_types[] =
            {
                    "FINISHED",
                    "ERROR",
                    "INFO",
                    "WARNING",
                    "DEBUG",
                    "ENHANCE",
                    "DEBUG_VERBOSE"
            };

    void log(type type_num, const std::string &msg) {
        if (type_num > LOG_LEVEL) {
            return;
        }
        if (use_file) {
            std::ofstream outfile;
            outfile.open(file, std::ios_base::app);
            outfile << "[" + message_types[type_num] + "]: " + msg;
        } else {
            printf("[%s]: %s", message_types[type_num].c_str(), msg.c_str());
        }
    }

    void log_stdout(type type_num, const std::string &msg) {
        if (type_num > LOG_LEVEL) {
            return;
        }
        printf("[%s]: %s", message_types[type_num].c_str(), msg.c_str());
    }
};


#endif //SATSOLVER_LOGGER_H
