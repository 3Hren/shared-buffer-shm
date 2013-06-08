#pragma once

#include "AbstractSharedBufferHandler.h"
#include "domain/Buffer.h"

class SharedBufferReader : public AbstractSharedBufferHandler
{
public:
    Buffer getBuffer(BufferId bufferId) const;
    BuffersDump getBuffersDump() const;
    QVector<TimeStamp> getTimestamps() const;
    //! --- BufferReader ---
    //! @todo: QualityCode getValueCode(BufferId id) const;

protected:
    SharedMemory::AccessMode getAcessMode() const override final;
};
