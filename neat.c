//
// Created by nitori on 14.03.17.
//
#include "neat.h"

int32_t next_inumber() {
    static int global_inumber = 0;
    global_inumber++;
    return global_inumber;
}


