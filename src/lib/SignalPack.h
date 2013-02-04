#pragma once

#include "SharedBufferGlobal.h"

#include <vector>

struct SignalPack {
    SignalPack(TimeStamp timeStamp, SignalValue *signalValues, int length) :
        timeStamp(timeStamp)
    {
        this->signalValues.assign(signalValues, signalValues + length);
    }

    TimeStamp timeStamp;
    std::vector<SignalValue> signalValues;
};
