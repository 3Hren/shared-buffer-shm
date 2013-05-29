#include "LowLevelBufferHandler.h"

#include <QDebug>

LowLevelBufferHandler::LowLevelBufferHandler(BufferId buffersCount, BufferPos bufferSize) :
    buffersCount(buffersCount),
    bufferSize(bufferSize)
{
}

BufferId LowLevelBufferHandler::getBuffersCount() const
{
    return buffersCount;
}

BufferPos LowLevelBufferHandler::getBufferSize() const
{
    return bufferSize;
}

quint16 LowLevelBufferHandler::getMetaDataSize() const
{
    return sizeof(MetaData);
}

quint32 LowLevelBufferHandler::getBufferDataSize() const
{
    return buffersCount * bufferSize * sizeof(SignalValue);
}

quint32 LowLevelBufferHandler::getQualityDataSize() const
{
    return buffersCount * sizeof(ValidityCode);
}

quint32 LowLevelBufferHandler::getTimestampDataSize() const
{
    return bufferSize * sizeof(TimeStamp);
}

int LowLevelBufferHandler::getDataLength() const
{
    return getMetaDataSize() + getBufferDataSize() + getQualityDataSize() + getTimestampDataSize();
}

int LowLevelBufferHandler::getDumpLength() const
{
    return getDataLength() - sizeof(BufferPos);
}

void *LowLevelBufferHandler::createStorage() const
{
    const int length = getDataLength();
    char *storage = new char[length];
    memset(storage, 0, length);

    MetaData meta;
    meta.currentPos = bufferSize - 1;
    memcpy(storage, &meta, sizeof(MetaData));
    return (void*)storage;
}

void LowLevelBufferHandler::push(TimeStamp timestamp, const SignalValue *signalsPack, const void *data) const
{
    char *metaData = (char*)data;
    char *bufferData = metaData + getMetaDataSize();
    char *qualityData = bufferData + getBufferDataSize();
    char *timestampData = qualityData + getQualityDataSize();

    MetaData meta;
    memcpy(&meta, metaData, getMetaDataSize());
    meta.currentPos++;
    Q_ASSERT(meta.currentPos <= bufferSize);
    if (meta.currentPos == bufferSize)
        meta.currentPos = 0;
    memcpy(metaData, &meta, getMetaDataSize());

    for (int bufferId = 0; bufferId < buffersCount; ++bufferId)
        memcpy(bufferData + (bufferId * bufferSize + meta.currentPos) * sizeof(SignalValue), &signalsPack[bufferId], sizeof(SignalValue));

    memcpy(timestampData + meta.currentPos * sizeof(TimeStamp), &timestamp, sizeof(TimeStamp));
}

char *LowLevelBufferHandler::getBuffersDump(const void *data) const
{        
    char *metaData = (char*)data;
    char *bufferData = metaData + getMetaDataSize();
    char *qualityData = bufferData + getBufferDataSize();
    char *timestampData = qualityData + getQualityDataSize();

    int length = getDumpLength();
    char *result = new char[length];
    char *resultBufferData = result;
    char *resultQualityData = resultBufferData + getBufferDataSize();
    char *resultTimestampsData = resultQualityData + getQualityDataSize();
    memset(result, 0, length);

    MetaData meta;
    memcpy(&meta, metaData, getMetaDataSize());

    for (BufferId bufferId = 0; bufferId < buffersCount; ++bufferId) {
        memcpy(resultBufferData + bufferId * bufferSize * sizeof(SignalValue),
               bufferData + bufferId * bufferSize * sizeof(SignalValue) + (meta.currentPos + 1) * sizeof(SignalValue),
               bufferSize * sizeof(SignalValue) - (meta.currentPos + 1)* sizeof(SignalValue));
        memcpy(resultBufferData + bufferId * bufferSize * sizeof(SignalValue) + bufferSize * sizeof(SignalValue) - (meta.currentPos + 1)* sizeof(SignalValue),
               bufferData + bufferId * bufferSize * sizeof(SignalValue),
               (meta.currentPos + 1)* sizeof(SignalValue));
    }

    // Reversing
    for (BufferId bufferId = 0; bufferId < buffersCount; ++bufferId) {
        SignalValue *buffersTmp = (SignalValue*)(resultBufferData + bufferId * bufferSize * sizeof(SignalValue));
        for (BufferId i = 0; i < bufferSize / 2; ++i) {
            SignalValue tmp = buffersTmp[i];
            buffersTmp[i] = buffersTmp[bufferSize - 1 - i];
            buffersTmp[bufferSize - 1 - i] = tmp;
        }
    }

    memcpy(resultQualityData, qualityData, getQualityDataSize());

    memcpy(resultTimestampsData,
           timestampData + (meta.currentPos + 1)* sizeof(TimeStamp),
           getTimestampDataSize() - (meta.currentPos + 1)* sizeof(TimeStamp));
    memcpy(resultTimestampsData + getTimestampDataSize() - (meta.currentPos + 1) * sizeof(TimeStamp),
           timestampData,
           (meta.currentPos + 1) * sizeof(TimeStamp));

    // Reversing
    TimeStamp *timestampTmp = (TimeStamp*)(resultTimestampsData);
    for (BufferId i = 0; i < bufferSize / 2; ++i) {
        TimeStamp tmp = timestampTmp[i];
        timestampTmp[i] = timestampTmp[bufferSize - 1 - i];
        timestampTmp[bufferSize - 1 - i] = tmp;
    }

    return result;
}
