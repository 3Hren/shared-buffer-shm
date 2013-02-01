#pragma once

#include "SharedBufferGlobal.h"

class BufferManager
{
    const BufferId buffersCount;
    const BufferSize bufferSize;
public:
    BufferManager(BufferId buffersCount, BufferSize bufferSize);

    BufferId getBuffersCount() const;
    BufferSize getBufferSize() const;

    int getDataLength() const;
    int getDumpLength() const;

    void *createStorage() const;
    void push(TimeStamp timestamp, SignalValue *signalsPack, void *data) const;
    void *getBuffersDump(const void *data) const;
    //float *getBuffer() const;
};
