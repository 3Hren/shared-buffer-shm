#include "AbstractSharedBufferHandler.h"

#include "BeanFactory.h"

#include "LowLevelBufferHandler.h"
#include "SharedMemory.h"

#include "SharedBufferException.h"

#include <log4cxx/logger.h>

AbstractSharedBufferHandler::AbstractSharedBufferHandler(BufferId buffersCount, BufferPos bufferSize)
{
    BeanFactory *factory = BeanFactory::getInstance();
    lowLevelBufferHandler = factory->createLowLevelBufferHandler(buffersCount, bufferSize);
    sharedMemory = factory->createSharedMemoryBean();
}

AbstractSharedBufferHandler::~AbstractSharedBufferHandler()
{
    delete lowLevelBufferHandler;
    delete sharedMemory;
}

BufferId AbstractSharedBufferHandler::getBuffersCount() const
{
    return lowLevelBufferHandler->getBuffersCount();
}

BufferPos AbstractSharedBufferHandler::getBufferSize() const
{
    return lowLevelBufferHandler->getBufferSize();
}

void AbstractSharedBufferHandler::attach(const QString &key)
{
    if (sharedMemory->isAttached())
        throw SharedBufferAlreadyAttachedException();

    sharedMemory->setKey(key);
    if (!sharedMemory->attach(getAcessMode())) {
        LOG4CXX_FATAL(log4cxx::Logger::getRootLogger(), "Shared memory segment attaching failed: " << sharedMemory->getErrorDescription().toStdString());
        throw SharedBufferNotAttachedException(sharedMemory->getErrorDescription());
    }

    LOG4CXX_INFO(log4cxx::Logger::getRootLogger(), "Shared memory segment has been successfully attached");
}
