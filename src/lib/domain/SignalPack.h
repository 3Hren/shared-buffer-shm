#pragma once

#include "SharedBufferGlobal.h"

#include <vector>

template<template<typename ...> class Container>
struct SignalPackType {
    TimeStamp timeStamp;
    Container<SignalValue> signalValues;
};

struct SignalPack : SignalPackType<std::vector> {
    SignalPack(TimeStamp timeStamp, SignalValue *signalValues, int length) {
        this->timeStamp = timeStamp;
        this->signalValues.assign(signalValues, signalValues + length);
    }
};
