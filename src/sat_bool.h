//
// Created by natalia on 31.05.23.
//

#ifndef SATSOLVER_SAT_BOOL_H
#define SATSOLVER_SAT_BOOL_H
enum class sat_bool : int {True = 1, False = -1, Undef = 0};


inline sat_bool operator! (sat_bool val) {
    return static_cast<sat_bool>(-static_cast<int>(val));
}
#endif //SATSOLVER_SAT_BOOL_H
