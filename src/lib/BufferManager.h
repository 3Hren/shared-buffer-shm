#pragma once

#include "SignalPack.h"

#include <queue>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

class SharedBufferWriter;
class BufferManager
{
    SharedBufferWriter *manager;

    std::queue<SignalPack> signalPacks;
    boost::mutex packsMutex;
    boost::condition packsAvailableCondition;
public:
    BufferManager(SharedBufferWriter *manager);

    void start();

    void push(TimeStamp timeStamp, SignalValue *signalValues);
};
