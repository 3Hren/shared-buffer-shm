#include "SharedBufferWriter.h"

#include "LowLevelBufferHandler.h"
#include "SharedMemory.h"
#include "exceptions/SharedBufferException.h"

void SharedBufferWriter::push(TimeStamp timestamp, const SignalValue *signalsPack) const
{
    if (!sharedMemory->isAttached())
        throw SharedBufferNotAttachedException(sharedMemory->getErrorDescription());

    sharedMemory->lock();
    lowLevelBufferHandler->push(timestamp, signalsPack, sharedMemory->data());
    sharedMemory->unlock();
}

AccessMode SharedBufferWriter::getAcessMode() const
{
    return AccessMode::ReadWrite;
}
