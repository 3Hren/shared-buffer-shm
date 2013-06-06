#pragma once

#include "AbstractSharedBufferHandler.h"

class SharedBufferWriter : public AbstractSharedBufferHandler
{
public:
    void push(TimeStamp timestamp, const SignalValue *signalsPack) const;    
    //! @todo void setQualityCode(BufferId id, QualityCode code) const;

protected:
    SharedMemory::AccessMode getAcessMode() const override final;
};
