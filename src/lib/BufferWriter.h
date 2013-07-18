#pragma once

#include <atomic>
#include <thread>

#include <log4cxx/logger.h>

#include "SharedBufferGlobal.h"
#include <ThreadSafeQueue.h>

struct SignalPack;
class SharedBufferWriter;
class BufferWriter
{
    std::thread consumer;
    std::atomic_bool done;
    ThreadSafeQueue<SignalPack> queue;
    SharedBufferWriter *sharedBufferWriter;
    log4cxx::LoggerPtr log;
public:
    BufferWriter(SharedBufferWriter *sharedBufferWriter);
    ~BufferWriter();

    bool isRunning() const;

    void start();

    void push(TimeStamp timeStamp, SignalValue *signalValues);
    void push(const SignalPack &pack);
    //! @todo: void setQualityCode()
    //! @todo: @feature: Можно сделать установку признака достоверности трехэтапной (добавить один бит):
    //! В случае если признак изменяется с 0 на любой, то изменение мгновенное
    //! Если же изменяется с любого на 0, то присваивается бит ожидания. Бит сохраняется до тех пор, пока не будет записано ровно bufferSize значений,
    //! после чего бит обнуляется.
    //! Либо хранить структуру вида {uint quality (изменяется мгновенно), bool corrupted (clearCount < bufferSize), int clearCount (число "чистых" значений)}
};
