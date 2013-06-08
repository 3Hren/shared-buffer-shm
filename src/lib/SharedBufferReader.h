#pragma once

#include "AbstractSharedBufferHandler.h"
#include "domain/Buffer.h"

class SharedBufferReader : public AbstractSharedBufferHandler
{
public:
    Buffer getBuffer(BufferId bufferId) const;
    //! --- BufferReader ---
    //! @todo: BuffersDump getBuffersDump() const &&;
    //! @todo: BuffersDump { vector<Buffer>; vector<TimeStamp>; }
    //! @todo: vector<TimeStamp> getTimeStamps() const;
    //! @todo: QualityCode getValueCode(BufferId id) const;

protected:
    SharedMemory::AccessMode getAcessMode() const override final;
};
