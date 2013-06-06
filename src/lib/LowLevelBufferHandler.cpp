#include "LowLevelBufferHandler.h"

#include <QDebug>

LowLevelBufferHandler::LowLevelBufferHandler(BufferId buffersCount, BufferPos bufferSize) :
    buffersCount(buffersCount),
    bufferSize(bufferSize),
    internal({
             sizeof(MetaData),
             bufferSize * sizeof(SignalValue),
             buffersCount * bufferSize * sizeof(SignalValue),
             buffersCount * sizeof(QualityCode),
             bufferSize * sizeof(TimeStamp)
             })
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

quint32 LowLevelBufferHandler::getDataLengthBytes() const
{
    return internal.getDataLengthBytes();
}

quint32 LowLevelBufferHandler::getDumpLengthBytes() const
{
    return internal.getDumpLengthBytes();
}

void *LowLevelBufferHandler::createStorage() const
{
    const int length = getDataLengthBytes();
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
    char *buffersData = metaData + internal.META_DATA_SIZE_BYTES;
    char *qualityData = buffersData + internal.BUFFERS_DATA_SIZE_BYTES;
    char *timestampData = qualityData + internal.QUALITY_DATA_SIZE_BYTES;

    MetaData meta;
    memcpy(&meta, metaData, internal.META_DATA_SIZE_BYTES);
    meta.currentPos++;
    Q_ASSERT(meta.currentPos <= bufferSize);
    if (meta.currentPos == bufferSize)
        meta.currentPos = 0;
    memcpy(metaData, &meta, internal.META_DATA_SIZE_BYTES);

    for (int bufferId = 0; bufferId < buffersCount; ++bufferId)
        memcpy(buffersData + (bufferId * bufferSize + meta.currentPos) * sizeof(SignalValue), &signalsPack[bufferId], sizeof(SignalValue));

    memcpy(timestampData + meta.currentPos * sizeof(TimeStamp), &timestamp, sizeof(TimeStamp));
}

char *LowLevelBufferHandler::getBuffersDump(const void *data) const
{        
    char *metaData = (char*)data;
    char *buffersData = metaData + internal.META_DATA_SIZE_BYTES;
    char *qualityData = buffersData + internal.BUFFERS_DATA_SIZE_BYTES;
    char *timestampData = qualityData + internal.QUALITY_DATA_SIZE_BYTES;

    int length = getDumpLengthBytes();
    char *result = new char[length];
    char *resultBuffersData = result;
    char *resultQualityData = resultBuffersData + internal.BUFFERS_DATA_SIZE_BYTES;
    char *resultTimestampsData = resultQualityData + internal.QUALITY_DATA_SIZE_BYTES;
    memset(result, 0, length);

    MetaData meta;
    memcpy(&meta, metaData, internal.META_DATA_SIZE_BYTES);

    for (BufferId bufferId = 0; bufferId < buffersCount; ++bufferId) {
        memcpy(resultBuffersData + bufferId * bufferSize * sizeof(SignalValue),
               buffersData + bufferId * bufferSize * sizeof(SignalValue) + (meta.currentPos + 1) * sizeof(SignalValue),
               bufferSize * sizeof(SignalValue) - (meta.currentPos + 1) * sizeof(SignalValue));
        memcpy(resultBuffersData + bufferId * bufferSize * sizeof(SignalValue) + bufferSize * sizeof(SignalValue) - (meta.currentPos + 1) * sizeof(SignalValue),
               buffersData + bufferId * bufferSize * sizeof(SignalValue),
               (meta.currentPos + 1) * sizeof(SignalValue));
    }

    // Reversing
    for (BufferId bufferId = 0; bufferId < buffersCount; ++bufferId)
        reverse<SignalValue>(resultBuffersData + bufferId * bufferSize * sizeof(SignalValue));

    memcpy(resultQualityData, qualityData, internal.QUALITY_DATA_SIZE_BYTES);

    memcpy(resultTimestampsData,
           timestampData + (meta.currentPos + 1)* sizeof(TimeStamp),
           internal.TIMESTAMP_DATA_SIZE_BYTES - (meta.currentPos + 1)* sizeof(TimeStamp));
    memcpy(resultTimestampsData + internal.TIMESTAMP_DATA_SIZE_BYTES - (meta.currentPos + 1) * sizeof(TimeStamp),
           timestampData,
           (meta.currentPos + 1) * sizeof(TimeStamp));

    reverse<TimeStamp>(resultTimestampsData);
    return result;
}

SignalValue *LowLevelBufferHandler::getBuffer(BufferId bufferId, const void *data) const
{
    char *metaData = (char*)data;
    char *buffersData = metaData + internal.META_DATA_SIZE_BYTES;

    int length = internal.BUFFER_DATA_SIZE_BYTES;
    char *result = new char[length];
    char *resultBuffersData = result;
    memset(result, 0, length);

    MetaData meta;
    memcpy(&meta, metaData, internal.META_DATA_SIZE_BYTES);

    memcpy(resultBuffersData,
           buffersData + bufferId * bufferSize * sizeof(SignalValue) + (meta.currentPos + 1) * sizeof(SignalValue),
           bufferSize * sizeof(SignalValue) - (meta.currentPos + 1) * sizeof(SignalValue));
    memcpy(resultBuffersData + bufferSize * sizeof(SignalValue) - (meta.currentPos + 1) * sizeof(SignalValue),
           buffersData + bufferId * bufferSize * sizeof(SignalValue),
           (meta.currentPos + 1) * sizeof(SignalValue));

    reverse<SignalValue>(result);
    return (SignalValue*)result;
}

TimeStamp *LowLevelBufferHandler::getTimeStamps(const void *data) const
{
    char *metaData = (char*)data;
    char *buffersData = metaData + internal.META_DATA_SIZE_BYTES;
    char *qualityData = buffersData + internal.BUFFERS_DATA_SIZE_BYTES;
    char *timestampData = qualityData + internal.QUALITY_DATA_SIZE_BYTES;

    char *result = new char[internal.TIMESTAMP_DATA_SIZE_BYTES];
    memset(result, 0, internal.TIMESTAMP_DATA_SIZE_BYTES);

    MetaData meta;
    memcpy(&meta, metaData, internal.META_DATA_SIZE_BYTES);
    memcpy(result,
           timestampData + (meta.currentPos + 1) * sizeof(TimeStamp),
           internal.TIMESTAMP_DATA_SIZE_BYTES - (meta.currentPos + 1) * sizeof(TimeStamp));
    memcpy(result + internal.TIMESTAMP_DATA_SIZE_BYTES - (meta.currentPos + 1) * sizeof(TimeStamp),
           timestampData,
           (meta.currentPos + 1) * sizeof(TimeStamp));

    reverse<TimeStamp>(result);
    return (TimeStamp*)result;
}

QualityCode LowLevelBufferHandler::getQualityCode(BufferId bufferId, const void *data) const
{
    char *metaData = (char*)data;
    char *buffersData = metaData + internal.META_DATA_SIZE_BYTES;
    char *qualityData = buffersData + internal.BUFFERS_DATA_SIZE_BYTES;
    QualityCode result;
    memcpy(&result, qualityData + sizeof(QualityCode) * bufferId, sizeof(QualityCode));
    return result;
}

void LowLevelBufferHandler::setQualityCode(BufferId bufferId, QualityCode code, const void *data) const
{
    char *metaData = (char*)data;
    char *buffersData = metaData + internal.META_DATA_SIZE_BYTES;
    char *qualityData = buffersData + internal.BUFFERS_DATA_SIZE_BYTES;
    memcpy(qualityData + sizeof(QualityCode) * bufferId, &code, sizeof(QualityCode));
}
