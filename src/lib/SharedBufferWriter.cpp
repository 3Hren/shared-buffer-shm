#include "SharedBufferWriter.h"

#include "LowLevelBufferHandler.h"
#include "SharedBufferException.h"

#include <QSharedMemory>
#include <QDebug>

SharedBufferWriter::SharedBufferWriter(LowLevelBufferHandler *lowLevelBufferHandler) :
    AbstractSharedBufferHandler(lowLevelBufferHandler)
{
}

void SharedBufferWriter::push(TimeStamp timestamp, const SignalValue *signalsPack) const
{
    if (!sharedMemory->isAttached())
        throw SharedBufferNotAttachedException(sharedMemory->errorString());

    sharedMemory->lock();
    lowLevelBufferHandler->push(timestamp, signalsPack, sharedMemory->data());
    sharedMemory->unlock();
}

AbstractSharedBufferHandler::AccessMode SharedBufferWriter::getAcessMode() const
{
    return AccessMode::ReadWrite;
}
