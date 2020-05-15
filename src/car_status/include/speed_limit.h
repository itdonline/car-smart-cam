#ifndef SPEED_LIMIT_H
#define SPEED_LIMIT_H

#include "timer.h"

#define SPEED_LIMIT_VALID_TIME 15 * 60 * 1000

struct MaxSpeedLimit {
    int speed_limit = -1;
    Timer::time_point_t  begin_time;
    bool has_warned = true; // set this to false to trigger a warning
};

#endif