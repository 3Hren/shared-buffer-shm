#pragma once

#include "SharedBufferGlobal.h"

class QSharedMemory;
class LowLevelBufferHandler;
class AbstractSharedBufferHandler
{
protected:
    LowLevelBufferHandler *lowLevelBufferHandler;
    QSharedMemory *sharedMemory;

public:
    enum class AccessMode {
        ReadOnly = 0,
        ReadWrite = 1
    };

    AbstractSharedBufferHandler(LowLevelBufferHandler *lowLevelBufferHandler);
    virtual ~AbstractSharedBufferHandler();

    BufferId getBuffersCount() const;
    BufferPos getBufferSize() const;

    void attach(const QString &key);

protected:
    virtual AccessMode getAcessMode() const = 0;
};
