#include "AbstractSharedBufferHandler.h"

#include "LowLevelBufferHandler.h"

#include "SharedBufferException.h"

#include <QSharedMemory>

#include <QDebug>

AbstractSharedBufferHandler::AbstractSharedBufferHandler(LowLevelBufferHandler *lowLevelBufferHandler) :
    lowLevelBufferHandler(lowLevelBufferHandler),
    sharedMemory(new QSharedMemory)
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
    if (!sharedMemory->attach(static_cast<QSharedMemory::AccessMode>(getAcessMode())))
        throw SharedBufferNotAttachedException(sharedMemory->errorString());
}
