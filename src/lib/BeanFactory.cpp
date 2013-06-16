#include "BeanFactory.h"

#include "QtBasedSharedMemory.h"
#include "LowLevelBufferHandler.h"

QAtomicPointer<BeanFactory> BeanFactory::self = 0;

BeanFactory *BeanFactory::getInstance() {
    Q_ASSERT_X(self, Q_FUNC_INFO, "BeanFactory pointer is null - you must register factory via 'registerFactory(BeanFactory *factory)' method");
    return self;
}

void BeanFactory::registerFactory(BeanFactory *factory) {
    Q_ASSERT_X(factory, Q_FUNC_INFO, "Factory pointer must not be null");
    self.testAndSetRelaxed(0, factory);
}

SharedMemory *BeanFactory::createSharedMemoryBean() const
{
    return new QtBasedSharedMemory;
}

LowLevelBufferHandler *BeanFactory::createLowLevelBufferHandler(BufferId buffersCount, BufferPos bufferSize) const
{
    return new LowLevelBufferHandler(buffersCount, bufferSize);
}
