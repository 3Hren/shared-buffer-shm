#include "AbstractSharedBufferHandler.h"

#include "BeanFactory.h"

#include "LowLevelBufferHandler.h"
#include "SharedMemory.h"
#include "SharedMemoryLocker.h"
#include "exceptions/SharedBufferException.h"
#include <QDebug>

AbstractSharedBufferHandler::AbstractSharedBufferHandler() :
    sharedMemory(0),
    lowLevelBufferHandler(0)
{
}

AbstractSharedBufferHandler::~AbstractSharedBufferHandler()
{
    decreaseClientCount();
}

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
    Q_ASSERT_X(sharedMemory && lowLevelBufferHandler, Q_FUNC_INFO, "Pointers must not be null");

    if (sharedMemory->isAttached())
        throw SharedBufferAlreadyAttachedException();

    sharedMemory->setKey(key);
    if (!sharedMemory->attach(getAcessMode()))
        throw SharedBufferNotAttachedException(sharedMemory->getErrorDescription());

    increaseClientCount();
}

void AbstractSharedBufferHandler::increaseClientCount()
{
    SharedMemoryLocker<SharedMemory> lock(sharedMemory);
    MetaData meta = lowLevelBufferHandler->getMetaData(sharedMemory->constData());
    meta.clientCount++;
    lowLevelBufferHandler->setMetaData(meta, sharedMemory->data());
}

void AbstractSharedBufferHandler::decreaseClientCount()
{
    if (sharedMemory && lowLevelBufferHandler) {
        SharedMemoryLocker<SharedMemory> lock(sharedMemory);
        MetaData meta = lowLevelBufferHandler->getMetaData(sharedMemory->constData());
        meta.clientCount--;
        lowLevelBufferHandler->setMetaData(meta, sharedMemory->data());
    }
}
