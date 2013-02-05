#include "AbstractSharedBufferHandler.h"

#include "BeanFactory.h"

#include "LowLevelBufferHandler.h"
#include "SharedMemory.h"

#include "SharedBufferException.h"

#include <log4cxx/logger.h>

void AbstractSharedBufferHandler::setLowLevelBufferHandler(LowLevelBufferHandler *lowLevelBufferHandler)
{
    this->lowLevelBufferHandler = lowLevelBufferHandler;
}

void AbstractSharedBufferHandler::setSharedMemory(SharedMemory *sharedMemory)
{
    this->sharedMemory = sharedMemory;
}

BufferId AbstractSharedBufferHandler::getBuffersCount() const
{
    return lowLevelBufferHandler->getBuffersCount();
}

BufferPos AbstractSharedBufferHandler::getBufferSize() const
{
    return lowLevelBufferHandler->getBufferSize();
}

SharedMemory *AbstractSharedBufferHandler::getSharedMemory() const
{
    return sharedMemory;
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
