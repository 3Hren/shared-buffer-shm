#pragma once

#include "SharedBufferGlobal.h"

#include <vector>

template<template<typename ...> class Vector>
struct SignalPackType {
    TimeStamp timeStamp;
    Vector<SignalValue> signalValues;
};

struct SignalPack : SignalPackType<std::vector> {
    SignalPack() {}
    SignalPack(TimeStamp timeStamp, SignalValue *signalValues, int length) {
        this->timeStamp = timeStamp;
        this->signalValues.assign(signalValues, signalValues + length);
    }
};
