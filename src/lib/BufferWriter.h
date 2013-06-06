#pragma once

#include "SignalPack.h"

#include <atomic>
#include <thread>

#include <ThreadSafeQueue.h>

class SharedBufferWriter;
class BufferWriter
{
    std::thread consumer;
    std::atomic_bool done;
    ThreadSafeQueue<SignalPack> queue;
    SharedBufferWriter *sharedBufferWriter;
public:
    BufferWriter(SharedBufferWriter *sharedBufferWriter);
    ~BufferWriter();

    void start();

    void push(TimeStamp timeStamp, SignalValue *signalValues);
    //! @todo: void push(const SignalPack &pack);
    //! @todo: void setQualityCode() -> delegate
};
