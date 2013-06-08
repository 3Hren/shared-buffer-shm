#pragma once

#include "AbstractSharedBufferHandler.h"
#include "domain/Buffer.h"

class SharedBufferReader : public AbstractSharedBufferHandler
{
public:
    Buffer getBuffer(BufferId bufferId) const;
    BuffersDump getBuffersDump() const;
    QVector<TimeStamp> getTimestamps() const;
    QualityCode getQualityCode(BufferId bufferId) const;

protected:
    SharedMemory::AccessMode getAcessMode() const override final;
};
