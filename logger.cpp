//
// Created by natalia on 31.07.23.
//
#include "logger.h"

void logger::log(type type_num, const std::string &msg) {
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

void logger::log_stdout(type type_num, const std::string &msg) {
    if (type_num > LOG_LEVEL) {
        return;
    }
    printf("[%s]: %s", message_types[type_num].c_str(), msg.c_str());
}