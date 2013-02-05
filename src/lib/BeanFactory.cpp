#include "BeanFactory.h"

#include "QtBasedSharedMemory.h"
#include "LowLevelBufferHandler.h"

QAtomicPointer<BeanFactory> BeanFactory::self = 0;

BeanFactory *BeanFactory::getInstance() {
    Q_ASSERT_X(self, Q_FUNC_INFO, u8"Указатель BeanFactory нулевой - зарегистрируйте фабрику через registerFactory(BeanFactory *factory)");
    return self;
}

void BeanFactory::registerFactory(BeanFactory *factory) {
    Q_ASSERT_X(factory, Q_FUNC_INFO, u8"Указатель factory нулевой!");
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
