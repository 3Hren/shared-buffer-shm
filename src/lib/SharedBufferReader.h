#pragma once

#include "AbstractSharedBufferHandler.h"

class SharedBufferReader : public AbstractSharedBufferHandler
{
public:
    SharedBufferReader(LowLevelBufferHandler *lowLevelBufferHandler);

protected:
    AccessMode getAcessMode() const override final;
};
