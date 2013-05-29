#pragma once

#include "SharedBufferGlobal.h"

struct MetaData {
    BufferPos currentPos;
    char empty[124];

    MetaData() : currentPos(0) {
        for (int i = 0; i < 124; ++i)
            empty[i] = 0;
    }
};
static_assert(sizeof(MetaData) == 128, "sizeof(MetaData) must be 128");


class LowLevelBufferHandler
{
    const BufferId buffersCount;
    const BufferPos bufferSize;
public:
    LowLevelBufferHandler(BufferId buffersCount, BufferPos bufferSize);
    virtual ~LowLevelBufferHandler() {}

    BufferId getBuffersCount() const;
    BufferPos getBufferSize() const;

    quint16 getMetaDataSize() const;
    quint32 getBufferDataSize() const;
    quint32 getQualityDataSize() const;
    quint32 getTimestampDataSize() const;
    int getDataLength() const;
    int getDumpLength() const;

    virtual void *createStorage() const;
    virtual void push(TimeStamp timestamp, const SignalValue *signalsPack, const void *data) const;
    virtual char *getBuffersDump(const void *data) const;
    //! @todo: SignalValue *getBuffer(BufferId id, const void *data) const;
    //! @todo: TimeStamp *getTimeStamps(const void *data) const;
    //! @todo: ValidityCode getValidityCode(BufferId id, const void *data) const;
    //! @todo: void setValidityCode(BufferId id, ValidityCode code, const void *data) const;
};
