#pragma once

#include "SharedBufferGlobal.h"

class QSharedMemory;
class LowLevelBufferHandler;
class SharedBufferWriter
{
    LowLevelBufferHandler *manager;
    QSharedMemory *shared;
public:
    SharedBufferWriter(LowLevelBufferHandler *manager);
    ~SharedBufferWriter();

    BufferId getBuffersCount() const;

    void attach(const QString &key);

    void push(TimeStamp timestamp, const SignalValue *signalsPack) const;       
};
