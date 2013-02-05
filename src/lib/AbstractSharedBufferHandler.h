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
    AbstractSharedBufferHandler(BufferId buffersCount, BufferPos bufferSize);
    virtual ~AbstractSharedBufferHandler();

    void setSharedMemory(SharedMemory *sharedMemory);

    BufferId getBuffersCount() const;
    BufferPos getBufferSize() const;

    SharedMemory *getSharedMemory() const;

    void attach(const QString &key);

protected:
    virtual SharedMemory::AccessMode getAcessMode() const = 0;
};
