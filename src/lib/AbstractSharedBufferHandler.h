#pragma once

#include <log4cxx/logger.h>

#include "SharedBufferGlobal.h"
#include "domain/Memory.hpp"

class SharedMemory;
class LowLevelBufferHandler;
class AbstractSharedBufferHandler
{
    log4cxx::LoggerPtr log;
protected:
    LowLevelBufferHandler *lowLevelBufferHandler;
    SharedMemory *sharedMemory;

public:
    AbstractSharedBufferHandler();
    virtual ~AbstractSharedBufferHandler();

    void setLowLevelBufferHandler(LowLevelBufferHandler *lowLevelBufferHandler);
    void setSharedMemory(SharedMemory *sharedMemory);

    BufferId getBuffersCount() const;
    BufferPos getBufferSize() const;

    SharedMemory *getSharedMemory() const;

    bool isAttached() const;
    void attach(const QString &key);

protected:
    virtual AccessMode getAcessMode() const = 0;

private:
    void increaseClientCount();
    void decreaseClientCount();
};
