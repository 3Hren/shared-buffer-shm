#pragma once

#include "AbstractSharedBufferHandler.h"

class SharedBufferWriter : public AbstractSharedBufferHandler
{
public:
    SharedBufferWriter(LowLevelBufferHandler *lowLevelBufferHandler);

    void push(TimeStamp timestamp, const SignalValue *signalsPack) const;    
    //! @todo void setValidityCode(BufferId id, ValidityCode code) const;

protected:
    AccessMode getAcessMode() const override final;
};
