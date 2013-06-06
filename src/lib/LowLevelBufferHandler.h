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
    const struct Internal {
        const quint16 META_DATA_SIZE_BYTES;
        const quint32 BUFFER_DATA_SIZE_BYTES;
        const quint32 BUFFERS_DATA_SIZE_BYTES;
        const quint32 QUALITY_DATA_SIZE_BYTES;
        const quint32 TIMESTAMP_DATA_SIZE_BYTES;

        inline constexpr quint32 getDataLengthBytes() const {
            return META_DATA_SIZE_BYTES + BUFFERS_DATA_SIZE_BYTES + QUALITY_DATA_SIZE_BYTES + TIMESTAMP_DATA_SIZE_BYTES;
        }

        inline constexpr quint32 getDumpLengthBytes() const {
            return getDataLengthBytes() - META_DATA_SIZE_BYTES;
        }
    } internal;

public:
    LowLevelBufferHandler(BufferId buffersCount, BufferPos bufferSize);
    virtual ~LowLevelBufferHandler() {}

    BufferId getBuffersCount() const;
    BufferPos getBufferSize() const;

    quint32 getDataLengthBytes() const;
    quint32 getDumpLengthBytes() const;

    virtual void *createStorage() const;
    virtual void push(TimeStamp timestamp, const SignalValue *signalsPack, const void *data) const;
    virtual char *getBuffersDump(const void *data) const;
    virtual SignalValue *getBuffer(BufferId bufferId, const void *data) const;
    virtual TimeStamp *getTimeStamps(const void *data) const;
    QualityCode getQualityCode(BufferId bufferId, const void *data) const;
    void setQualityCode(BufferId bufferId, QualityCode code, const void *data) const;

private:
    template<typename T>
    void reverse(char *data) const;
};

template<typename T>
void LowLevelBufferHandler::reverse(char *data) const {
    T *tempArray = (T*)data;
    for (BufferId i = 0; i < bufferSize / 2; ++i) {
        T tmp = tempArray[i];
        tempArray[i] = tempArray[bufferSize - 1 - i];
        tempArray[bufferSize - 1 - i] = tmp;
    }
}
