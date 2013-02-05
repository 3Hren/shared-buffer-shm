#include "AbstractSharedBufferHandler.h"

#include "LowLevelBufferHandler.h"
#include "QtBasedSharedMemory.h"

#include "SharedBufferException.h"

#include <QDebug>

AbstractSharedBufferHandler::AbstractSharedBufferHandler(LowLevelBufferHandler *lowLevelBufferHandler) :
    lowLevelBufferHandler(lowLevelBufferHandler),
    sharedMemory(new QtBasedSharedMemory)
{
}

AbstractSharedBufferHandler::~AbstractSharedBufferHandler()
{
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
    if (!sharedMemory->attach(getAcessMode()))
        throw SharedBufferNotAttachedException(sharedMemory->getErrorDescription());
}
