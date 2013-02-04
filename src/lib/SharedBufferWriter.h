#pragma once

#include "SharedBufferGlobal.h"

class QSharedMemory;
class LowLevelBufferManager;
class SharedBufferWriter
{
    LowLevelBufferManager *manager;
    QSharedMemory *shared;
public:
    SharedBufferWriter(LowLevelBufferManager *manager);
    ~SharedBufferWriter();

    BufferId getBuffersCount() const;

    void attach(const QString &key);

    void push(TimeStamp timestamp, const SignalValue *signalsPack) const;       
};
