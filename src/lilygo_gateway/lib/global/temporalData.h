#ifndef TEMPORAL_DATA_CONSTANTS_H
#define TEMPORAL_DATA_CONSTANTS_H

#pragma once

#include <Arduino.h>

namespace SensorSensei::Constants::TemporalData
{
    extern u_int8_t CurrentDay;
    extern u_int8_t CurrentMonth;
    extern u_int8_t CurrentYear;

    extern u_int8_t CurrentHour;
    extern u_int8_t CurrentMinute;
    extern u_int8_t CurrentSecond;
}

#endif