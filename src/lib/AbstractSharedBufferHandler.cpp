#include "AbstractSharedBufferHandler.h"

#include "BeanFactory.h"

#include "LowLevelBufferHandler.h"
#include "SharedMemory.h"
#include "exceptions/SharedBufferException.h"

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

bool AbstractSharedBufferHandler::isAttached() const
{
    return sharedMemory->isAttached();
}

void AbstractSharedBufferHandler::attach(const QString &key)
{
    if (sharedMemory->isAttached())
        throw SharedBufferAlreadyAttachedException();

    sharedMemory->setKey(key);
    if (!sharedMemory->attach(getAcessMode()))
        throw SharedBufferNotAttachedException(sharedMemory->getErrorDescription());        
}
