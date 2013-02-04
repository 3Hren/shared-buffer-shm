#pragma once

#include "SharedBufferGlobal.h"

class LowLevelBufferHandler
{
    const BufferId buffersCount;
    const BufferPos bufferSize;
public:
    LowLevelBufferHandler(BufferId buffersCount, BufferPos bufferSize);

    BufferId getBuffersCount() const;
    BufferPos getBufferSize() const;

    int getDataLength() const;
    int getDumpLength() const;

    void *createStorage() const;
    void push(TimeStamp timestamp, const SignalValue *signalsPack, const void *data) const;
    char *getBuffersDump(const void *data) const;
    //! @todo: SignalValue *getBuffer(BufferId id) const;
    //! @todo: TimeStamp *getTimeStamps() const;
    //! @todo: ValidityCode getValidityCode(BufferId id) const;
};