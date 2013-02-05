#pragma once

#include "AbstractSharedBufferHandler.h"

class SharedBufferReader : public AbstractSharedBufferHandler
{
public:
    //! @todo: char *getBuffersDump() const;
    //! @todo: SignalValue *getBuffer(BufferId id, const void *data) const;
    //! @todo: TimeStamp *getTimeStamps(const void *data) const;
    //! @todo: ValidityCode getValidityCode(BufferId id, const void *data) const;

    //! --- BufferReader ---
    //! @todo: BuffersDump getBuffersDump() const &&;
    //! @todo: BuffersDump { vector<Buffer>; vector<TimeStamp>; }
    //! @todo: Buffer getBuffer(BufferId id) const;
    //! @todo: Buffer { vector<SignalValue>; ValidityCode code; }
    //! @todo: vector<TimeStamp> getTimeStamps() const;
    //! @todo: ValidityCode getValueCode(BufferId id) const;

protected:
    SharedMemory::AccessMode getAcessMode() const override final;
};
