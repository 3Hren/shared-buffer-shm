#include "SharedBufferReader.h"

SharedBufferReader::SharedBufferReader(BufferId buffersCount, BufferPos bufferSize) :
    AbstractSharedBufferHandler(buffersCount, bufferSize)
{
}

SharedMemory::AccessMode SharedBufferReader::getAcessMode() const
{
    return SharedMemory::AccessMode::ReadOnly;
}
