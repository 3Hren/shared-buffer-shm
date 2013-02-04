#pragma once

#include "SharedBufferGlobal.h"

class LowLevelBufferManager
{
    const BufferId buffersCount;
    const BufferPos bufferSize;
public:
    LowLevelBufferManager(BufferId buffersCount, BufferPos bufferSize);

    BufferId getBuffersCount() const;
    BufferPos getBufferSize() const;

    int getDataLength() const;
    int getDumpLength() const;

    void *createStorage() const;
    void push(TimeStamp timestamp, SignalValue *signalsPack, const void *data) const;
    char *getBuffersDump(const void *data) const;
    //! @todo: SignalValue *getBuffer(BufferId id) const;
    //! @todo: TimeStamp *getTimeStamps() const;
    //! @todo: ValidityCode getValidityCode(BufferId id) const;
};
