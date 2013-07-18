#pragma once

#include <QAtomicPointer>

#include "SharedBufferGlobal.h"

class SharedMemory;
class LowLevelBufferHandler;
class BeanFactory {
    static QAtomicPointer<BeanFactory> self;
public:
    BeanFactory() {}
    virtual ~BeanFactory() {}

    static BeanFactory *getInstance();
    static void registerFactory(BeanFactory *factory);

    virtual SharedMemory *createSharedMemoryBean() const;
    virtual LowLevelBufferHandler *createLowLevelBufferHandler(BufferId buffersCount, BufferPos bufferSize) const;
};
