//
// Created by natalia on 31.07.23.
//
#include "logger.h"
#include "lit.h"

void logger::log(type type_num, const std::string &msg) {
    if (type_num > LOG_LEVEL) {
        return;
    }
    if (use_file) {
        std::ofstream outfile;
        outfile.open(file, std::ios_base::app);
        outfile << "[" + message_types[type_num] + "]: " + msg;
    } else {
        printf("[%s]: %s \n", message_types[type_num].c_str(), msg.c_str());
    }
}

void logger::log_stdout(type type_num, const std::string &msg) {
    if (type_num > LOG_LEVEL) {
        return;
    }
    printf("[%s]: %s \n", message_types[type_num].c_str(), msg.c_str());
}

bool logger::cond_log(logger::type type_num) {
    return logger::LOG_LEVEL >= type_num;
}

std::string logger::vec_to_string(vector<lit> vec) {
    std::string ret_val = "{";
    std::string add;
    for (lit l : vec) {
        ret_val.append(l.to_string()).append(add).append(", ");
    }
    ret_val.pop_back();
    ret_val.pop_back();
    ret_val.append("}");
    return ret_val;
}
