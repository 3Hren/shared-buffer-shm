#include "SharedBufferReader.h"

SharedBufferReader::SharedBufferReader(LowLevelBufferHandler *lowLevelBufferHandler) :
    AbstractSharedBufferHandler(lowLevelBufferHandler)
{
}

SharedMemory::AccessMode SharedBufferReader::getAcessMode() const
{
    return SharedMemory::AccessMode::ReadOnly;
}
