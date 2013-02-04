#include "LowLevelBufferManager.h"

#include <QDebug>

#include <boost/timer/timer.hpp>

constexpr int TIMESTAMP_MULTIPLIER = sizeof(TimeStamp) / sizeof(SignalValue);

LowLevelBufferManager::LowLevelBufferManager(BufferId buffersCount, BufferPos bufferSize) :
    buffersCount(buffersCount),
    bufferSize(bufferSize)
{
}

BufferId LowLevelBufferManager::getBuffersCount() const
{
    return buffersCount;
}

BufferPos LowLevelBufferManager::getBufferSize() const
{
    return bufferSize;
}

int LowLevelBufferManager::getDataLength() const
{
    return sizeof(BufferPos) + buffersCount * bufferSize * sizeof(SignalValue) + buffersCount * sizeof(ValidityCode) + bufferSize * sizeof(TimeStamp);
}

int LowLevelBufferManager::getDumpLength() const
{
    return getDataLength() - sizeof(BufferPos);
}

void *LowLevelBufferManager::createStorage() const
{
    const int length = getDataLength();
    char *storage = new char[length];
    memset(storage, 0, length);

    const BufferPos currentId = bufferSize - 1;
    memcpy((float*)storage, &currentId, sizeof(BufferPos));
    return (void*)storage;
}

void LowLevelBufferManager::push(TimeStamp timestamp, const SignalValue *signalsPack, const void *data) const
{
    char *_data = (char*)data;

    BufferPos currentPos;
    memcpy(&currentPos, _data, sizeof(BufferPos));
    currentPos++;
    Q_ASSERT(currentPos <= bufferSize);
    if (currentPos == bufferSize)
        currentPos = 0;

    memcpy(_data, &currentPos, sizeof(BufferPos));
    for (int bufferId = 0; bufferId < buffersCount; ++bufferId) {
        memcpy(_data
               + sizeof(BufferPos)
               + (bufferId * bufferSize + currentPos) * sizeof(SignalValue), &signalsPack[bufferId], sizeof(SignalValue));
    }

    memcpy(_data
           + sizeof(BufferPos)
           + (buffersCount * bufferSize) * sizeof(SignalValue)
           + (buffersCount) * sizeof(ValidityCode)
           + (currentPos * sizeof(TimeStamp)), &timestamp, sizeof(TimeStamp));
}

char *LowLevelBufferManager::getBuffersDump(const void *data) const
{    
    int length = getDumpLength();
    char *result = new char[length];
    memset(result, 0, length);

    BufferPos currentPos;
    memcpy(&currentPos, data, sizeof(BufferPos));

    const BufferPos startPos = currentPos;
    for (BufferPos id = startPos, copyed = 0; copyed == 0 | id != startPos; --id, ++copyed) {
        for (BufferId count = 0; count < buffersCount; ++count)
            memcpy((char*)result + (count * bufferSize + copyed) * sizeof(SignalValue),
                   (char*)data + sizeof(BufferPos) + (count * bufferSize + id) * sizeof(SignalValue),
                   sizeof(SignalValue));

        memcpy((char*)result + (buffersCount * bufferSize) * sizeof(SignalValue) + (buffersCount) * sizeof(ValidityCode) + copyed * sizeof(TimeStamp),
               (char*)data + sizeof(BufferPos) + (buffersCount * bufferSize) * sizeof(SignalValue) + (buffersCount) * sizeof(ValidityCode) + id * sizeof(TimeStamp),
               sizeof(TimeStamp));

        if (id == 0)
            id = bufferSize;
    }

    memcpy((char*)result + (buffersCount * bufferSize) * sizeof(SignalValue),
           (char*)data + sizeof(BufferPos) + (buffersCount * bufferSize) * sizeof(SignalValue),
           buffersCount * sizeof(ValidityCode));

    return result;
}
