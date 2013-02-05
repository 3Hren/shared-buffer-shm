#pragma once

#include "SharedBufferGlobal.h"
#include "SharedMemory.h"

class LowLevelBufferHandler;
class AbstractSharedBufferHandler
{
protected:
    LowLevelBufferHandler *lowLevelBufferHandler;
    SharedMemory *sharedMemory;

public:
    AbstractSharedBufferHandler(LowLevelBufferHandler *lowLevelBufferHandler);
    virtual ~AbstractSharedBufferHandler();

    BufferId getBuffersCount() const;
    BufferPos getBufferSize() const;

    void attach(const QString &key);

protected:
    virtual SharedMemory::AccessMode getAcessMode() const = 0;
};
