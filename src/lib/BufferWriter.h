#pragma once

#include "SignalPack.h"

#include <queue>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

class SharedBufferWriter;
class BufferWriter
{
    const SharedBufferWriter * const manager;

    std::queue<SignalPack> signalPacks;
    boost::mutex packsMutex;
    boost::condition packsAvailableCondition;
public:
    BufferWriter(SharedBufferWriter *manager);

    void start();

    void push(TimeStamp timeStamp, SignalValue *signalValues);
    //! @todo: void push(const SignalPack &pack);
    //! @todo: void setValidityCode() -> delegate
};
