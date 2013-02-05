#include "SharedBufferReader.h"

SharedMemory::AccessMode SharedBufferReader::getAcessMode() const
{
    return SharedMemory::AccessMode::ReadOnly;
}
