#include "LowLevelBufferHandler.h"

#include "SharedBufferException.h"

#include <QDebug>

struct Internal {
    const quint16 META_DATA_SIZE_BYTES;
    const quint32 BUFFER_DATA_SIZE_BYTES;
    const quint32 BUFFERS_DATA_SIZE_BYTES;
    const quint32 QUALITY_DATA_SIZE_BYTES;
    const quint32 TIMESTAMP_DATA_SIZE_BYTES;

    const quint32 TIMESTAMPS_OFFSET;

    constexpr Internal(BufferPos size, BufferId count) :
        META_DATA_SIZE_BYTES(sizeof(MetaData)),
        BUFFER_DATA_SIZE_BYTES(size * sizeof(SignalValue)),
        BUFFERS_DATA_SIZE_BYTES(count * size * sizeof(SignalValue)),
        QUALITY_DATA_SIZE_BYTES(count * sizeof(QualityCode)),
        TIMESTAMP_DATA_SIZE_BYTES(size * sizeof(TimeStamp)),

        TIMESTAMPS_OFFSET(sizeof(MetaData) + count * size * sizeof(SignalValue) + count * sizeof(QualityCode))
    {}

    inline constexpr quint32 getDataLengthBytes() const {
        return META_DATA_SIZE_BYTES + BUFFERS_DATA_SIZE_BYTES + QUALITY_DATA_SIZE_BYTES + TIMESTAMP_DATA_SIZE_BYTES;
    }

    inline constexpr quint32 getDumpLengthBytes() const {
        return getDataLengthBytes() - META_DATA_SIZE_BYTES;
    }
};

LowLevelBufferHandler::LowLevelBufferHandler(BufferId buffersCount, BufferPos bufferSize) :
    buffersCount(buffersCount),
    bufferSize(bufferSize),
    internal(new Internal(bufferSize, buffersCount))
{
}

LowLevelBufferHandler::~LowLevelBufferHandler()
{
    delete internal;
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
    return internal->getDataLengthBytes();
}

quint32 LowLevelBufferHandler::getDumpLengthBytes() const
{
    return internal->getDumpLengthBytes();
}

void *LowLevelBufferHandler::createStorage() const
{
    const int length = getDataLengthBytes();
    char *storage = new char[length];
    memset(storage, 0, length);

    MetaData meta;
    meta.currentPos = bufferSize - 1;
    memcpy(storage, &meta, sizeof(MetaData));
    return reinterpret_cast<void *>(storage);
}

void LowLevelBufferHandler::push(TimeStamp timestamp, const SignalValue *signalsPack, void *data) const
{
    char *metaData = reinterpret_cast<char *>(data);
    char *buffersData = metaData + internal->META_DATA_SIZE_BYTES;
    char *qualityData = buffersData + internal->BUFFERS_DATA_SIZE_BYTES;
    char *timestampData = qualityData + internal->QUALITY_DATA_SIZE_BYTES;

    MetaData meta;
    memcpy(&meta, metaData, internal->META_DATA_SIZE_BYTES);
    meta.currentPos++;
    Q_ASSERT(meta.currentPos <= bufferSize);
    if (meta.currentPos == bufferSize)
        meta.currentPos = 0;
    memcpy(metaData, &meta, internal->META_DATA_SIZE_BYTES);

    for (BufferId bufferId = 0; bufferId < buffersCount; ++bufferId)
        memcpy(buffersData + (bufferId * bufferSize + meta.currentPos) * sizeof(SignalValue), &signalsPack[bufferId], sizeof(SignalValue));

    memcpy(timestampData + meta.currentPos * sizeof(TimeStamp), &timestamp, sizeof(TimeStamp));
}

char *LowLevelBufferHandler::getRawBuffersDump(const void *data) const
{        
    const char *metaData = reinterpret_cast<const char *>(data);
    const char *buffersData = metaData + internal->META_DATA_SIZE_BYTES;
    const char *qualityData = buffersData + internal->BUFFERS_DATA_SIZE_BYTES;
    const char *timestampData = qualityData + internal->QUALITY_DATA_SIZE_BYTES;

    int length = getDumpLengthBytes();
    char *result = new char[length];
    char *resultBuffersData = result;
    char *resultQualityData = resultBuffersData + internal->BUFFERS_DATA_SIZE_BYTES;
    char *resultTimestampsData = resultQualityData + internal->QUALITY_DATA_SIZE_BYTES;
    memset(result, 0, length);

    MetaData meta;
    memcpy(&meta, metaData, internal->META_DATA_SIZE_BYTES);

    for (BufferId bufferId = 0; bufferId < buffersCount; ++bufferId) {
        memcpy(resultBuffersData + bufferId * bufferSize * sizeof(SignalValue),
               buffersData + bufferId * bufferSize * sizeof(SignalValue) + (meta.currentPos + 1) * sizeof(SignalValue),
               bufferSize * sizeof(SignalValue) - (meta.currentPos + 1) * sizeof(SignalValue));
        memcpy(resultBuffersData + bufferId * bufferSize * sizeof(SignalValue) + bufferSize * sizeof(SignalValue) - (meta.currentPos + 1) * sizeof(SignalValue),
               buffersData + bufferId * bufferSize * sizeof(SignalValue),
               (meta.currentPos + 1) * sizeof(SignalValue));
    }

    for (BufferId bufferId = 0; bufferId < buffersCount; ++bufferId)
        reverse<SignalValue>(resultBuffersData + bufferId * bufferSize * sizeof(SignalValue));

    memcpy(resultQualityData, qualityData, internal->QUALITY_DATA_SIZE_BYTES);

    memcpy(resultTimestampsData,
           timestampData + (meta.currentPos + 1)* sizeof(TimeStamp),
           internal->TIMESTAMP_DATA_SIZE_BYTES - (meta.currentPos + 1)* sizeof(TimeStamp));
    memcpy(resultTimestampsData + internal->TIMESTAMP_DATA_SIZE_BYTES - (meta.currentPos + 1) * sizeof(TimeStamp),
           timestampData,
           (meta.currentPos + 1) * sizeof(TimeStamp));

    reverse<TimeStamp>(resultTimestampsData);
    return result;
}

SignalValue *LowLevelBufferHandler::getRawBuffer(BufferId bufferId, const void *data) const
{
    checkBufferId(bufferId);
    char *raw = new char[internal->BUFFER_DATA_SIZE_BYTES];
    SignalValue *buffer = reinterpret_cast<SignalValue*>(raw);
    parseBuffer(bufferId, data, buffer);
    return buffer;
}

QVector<SignalValue> LowLevelBufferHandler::getBuffer(BufferId bufferId, const void *data) const
{
    return getBuffer<QVector>(bufferId, data);
}

TimeStamp *LowLevelBufferHandler::getRawTimeStamps(const void *data) const
{
    char *result = new char[internal->TIMESTAMP_DATA_SIZE_BYTES];
    TimeStamp *timestamps = reinterpret_cast<TimeStamp *>(result);
    parseTimestamps(data, timestamps);
    return timestamps;
}

QVector<TimeStamp> LowLevelBufferHandler::getTimestamps(const void *data) const
{
    return getTimestamps<QVector>(data);
}

QualityCode LowLevelBufferHandler::getQualityCode(BufferId bufferId, const void *data) const
{
    checkBufferId(bufferId);
    const char *metaData = reinterpret_cast<const char *>(data);
    const char *buffersData = metaData + internal->META_DATA_SIZE_BYTES;
    const char *qualityData = buffersData + internal->BUFFERS_DATA_SIZE_BYTES;
    QualityCode result;
    memcpy(&result, qualityData + sizeof(QualityCode) * bufferId, sizeof(QualityCode));
    return result;
}

void LowLevelBufferHandler::setQualityCode(BufferId bufferId, QualityCode code, void *data) const
{
    checkBufferId(bufferId);
    char *metaData = reinterpret_cast<char *>(data);
    char *buffersData = metaData + internal->META_DATA_SIZE_BYTES;
    char *qualityData = buffersData + internal->BUFFERS_DATA_SIZE_BYTES;
    memcpy(qualityData + sizeof(QualityCode) * bufferId, &code, sizeof(QualityCode));
}

void LowLevelBufferHandler::parseBuffer(BufferId bufferId, const void *from, SignalValue *to) const
{
    const char *metaData = reinterpret_cast<const char *>(from);
    const char *buffersData = metaData + internal->META_DATA_SIZE_BYTES;

    int length = internal->BUFFER_DATA_SIZE_BYTES;
    char *result = reinterpret_cast<char *>(to);
    char *resultBuffersData = result;
    memset(result, 0, length);

    MetaData meta;
    memcpy(&meta, metaData, internal->META_DATA_SIZE_BYTES);
    memcpy(resultBuffersData,
           buffersData + bufferId * bufferSize * sizeof(SignalValue) + (meta.currentPos + 1) * sizeof(SignalValue),
           bufferSize * sizeof(SignalValue) - (meta.currentPos + 1) * sizeof(SignalValue));
    memcpy(resultBuffersData + bufferSize * sizeof(SignalValue) - (meta.currentPos + 1) * sizeof(SignalValue),
           buffersData + bufferId * bufferSize * sizeof(SignalValue),
           (meta.currentPos + 1) * sizeof(SignalValue));

    reverse<SignalValue>(result);
}

void LowLevelBufferHandler::parseTimestamps(const void *from, TimeStamp *to) const
{
    const char *data = reinterpret_cast<const char *>(from);
    const char *metaData = data;
    const char *timestampData = data + internal->TIMESTAMPS_OFFSET;

    char *result = reinterpret_cast<char *>(to);
    memset(result, 0, internal->TIMESTAMP_DATA_SIZE_BYTES);

    MetaData meta;
    memcpy(&meta, metaData, internal->META_DATA_SIZE_BYTES);
    memcpy(result,
           timestampData + (meta.currentPos + 1) * sizeof(TimeStamp),
           internal->TIMESTAMP_DATA_SIZE_BYTES - (meta.currentPos + 1) * sizeof(TimeStamp));
    memcpy(result + internal->TIMESTAMP_DATA_SIZE_BYTES - (meta.currentPos + 1) * sizeof(TimeStamp),
           timestampData,
           (meta.currentPos + 1) * sizeof(TimeStamp));

    reverse<TimeStamp>(result);
}

void LowLevelBufferHandler::checkBufferId(BufferId bufferId) const
{
    if (bufferId >= buffersCount)
        throw IllegalValueException(QString("Buffer id (%1) should be in range [0, %2]").arg(bufferId).arg(buffersCount - 1).toStdString());
}
