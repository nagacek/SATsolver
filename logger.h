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
        DEBUG,
        ENHANCE,
        INFO,
        WARN,
        ERROR
    };
    std::string message_types[] =
            {
            "DEBUG",
            "ENHANCE",
            "INFO",
            "WARNING",
            "ERROR"
            };

    void log(type type_num, const std::string& msg) {
        if (use_file) {
            std::ofstream outfile;
            outfile.open(file, std::ios_base::app);
            outfile << "[" + message_types[type_num] + "]: " + msg;
        } else {
            printf("[%s]: %s", message_types[type_num].c_str(), msg.c_str());
        }
    }
};


#endif //SATSOLVER_LOGGER_H
