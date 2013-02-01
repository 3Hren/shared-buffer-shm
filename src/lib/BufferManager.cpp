#include "BufferManager.h"

#include <QDebug>

constexpr int TIMESTAMP_MULTIPLIER = sizeof(TimeStamp) / sizeof(SignalValue);

BufferManager::BufferManager(BufferId buffersCount, BufferSize bufferSize) :
    buffersCount(buffersCount),
    bufferSize(bufferSize)
{
    Q_ASSERT_X(sizeof(BufferSize) == sizeof(SignalValue), "Global", "sizeof(BufferSize) must be equal sizeof(SignalValue) because of memcopy operations");
}

BufferId BufferManager::getBuffersCount() const
{
    return buffersCount;
}

BufferSize BufferManager::getBufferSize() const
{
    return bufferSize;
}

int BufferManager::getDataLength() const
{
    return buffersCount * bufferSize * sizeof(SignalValue) + 1 * sizeof(SignalValue) + bufferSize * sizeof(TimeStamp);
}

int BufferManager::getDumpLength() const
{
    return getDataLength() - sizeof(SignalValue);
}

void *BufferManager::createStorage() const
{
    const int length = getDataLength();
    char *storage = new char[length];
    memset(storage, 0, length);

    float* _storage = (float*)storage;
    const BufferSize currentId = bufferSize - 1;
    memcpy(_storage + buffersCount * bufferSize, &currentId, sizeof(SignalValue));
    return (void*)storage;
}

void BufferManager::push(TimeStamp timestamp, SignalValue *signalsPack, void *data) const
{
    SignalValue *_data = (SignalValue*)data;

    BufferSize currentId;
    memcpy(&currentId, _data + buffersCount * bufferSize, sizeof(SignalValue));
    currentId++;
    Q_ASSERT(currentId <= bufferSize);
    if (currentId == bufferSize)
        currentId = 0;

    for (int bufferId = 0; bufferId < buffersCount; ++bufferId)
        memcpy(_data + bufferId * bufferSize + currentId, &signalsPack[bufferId], sizeof(SignalValue));        
    memcpy(_data + buffersCount * bufferSize, &currentId, sizeof(SignalValue));
    memcpy(_data + buffersCount * bufferSize + 1 + currentId * (sizeof(TimeStamp) / sizeof(SignalValue)), &timestamp, sizeof(TimeStamp));        
}

void *BufferManager::getBuffersDump(const void *data) const
{    
    int length = getDumpLength();
    char *result = new char[length];
    memset(result, 0, length);

    SignalValue *_data = (SignalValue*)data;
    SignalValue *_result = (SignalValue*)result;
    BufferSize currentId;
    memcpy(&currentId, _data + buffersCount * bufferSize, sizeof(SignalValue));
    const BufferSize startId = currentId;
    for (BufferSize id = startId, copyed = 0; copyed == 0 | id != startId; --id, ++copyed) {
        for (BufferId count = 0; count < buffersCount; ++count)
            _result[bufferSize * count + copyed] = _data[count * bufferSize + id];

        memcpy(_result + buffersCount * bufferSize + copyed * TIMESTAMP_MULTIPLIER, _data + buffersCount * bufferSize + 1 + id * TIMESTAMP_MULTIPLIER, sizeof(TimeStamp));
        if (id == 0)
            id = bufferSize;
    }

    return (void*)result;
}
