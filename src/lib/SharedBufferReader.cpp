#include "SharedBufferReader.h"

SharedBufferReader::SharedBufferReader(LowLevelBufferHandler *lowLevelBufferHandler) :
    AbstractSharedBufferHandler(lowLevelBufferHandler)
{
}

AbstractSharedBufferHandler::AccessMode SharedBufferReader::getAcessMode() const
{
    return AccessMode::ReadOnly;
}
