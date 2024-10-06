#pragma once
#include <Arduino.h>

struct InfraredCode
{
    uint8_t protocol = 0;
    uint16_t address = 0;
    uint16_t command = 0;
    uint16_t numberOfBits = 0;
    uint16_t extra = 0;
};
