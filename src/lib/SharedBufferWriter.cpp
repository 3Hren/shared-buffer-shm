#include "SharedBufferWriter.h"

#include "LowLevelBufferHandler.h"
#include "SharedBufferException.h"

SharedBufferWriter::SharedBufferWriter(BufferId buffersCount, BufferPos bufferSize) :
    AbstractSharedBufferHandler(buffersCount, bufferSize)
{
}

void SharedBufferWriter::push(TimeStamp timestamp, const SignalValue *signalsPack) const
{
    if (!sharedMemory->isAttached())
        throw SharedBufferNotAttachedException(sharedMemory->getErrorDescription());

    sharedMemory->lock();
    lowLevelBufferHandler->push(timestamp, signalsPack, sharedMemory->data());
    sharedMemory->unlock();
}

SharedMemory::AccessMode SharedBufferWriter::getAcessMode() const
{
    return SharedMemory::AccessMode::ReadWrite;
}
