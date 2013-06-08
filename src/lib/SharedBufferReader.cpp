#include "SharedBufferReader.h"

#include "LowLevelBufferHandler.h"

#include <memory>
#include <cstring>

#include <QDebug>

Buffer SharedBufferReader::getBuffer(BufferId bufferId) const
{
    sharedMemory->lock();
    const void *data = sharedMemory->constData();
    QVector<SignalValue> values = lowLevelBufferHandler->getBuffer(bufferId, data);
    QualityCode quality = lowLevelBufferHandler->getQualityCode(bufferId, data);
    sharedMemory->unlock();
    const Buffer buffer{values, quality};
    return buffer;
}

BuffersDump SharedBufferReader::getBuffersDump() const
{
    const int length = lowLevelBufferHandler->getDataLengthBytes();
    std::unique_ptr<char[]> copy(new char[length]);

    sharedMemory->lock();
    std::memcpy(copy.get(), sharedMemory->constData(), length);
    sharedMemory->unlock();        

    const BufferId count = lowLevelBufferHandler->getBuffersCount();
    BuffersDump dump;
    dump.timestamps = lowLevelBufferHandler->getTimestamps(copy.get());
    dump.buffers.reserve(count);
    for (BufferId id = 0; id < count; ++id) {
        Buffer buffer;
        buffer.values = lowLevelBufferHandler->getBuffer(id, copy.get());
        buffer.quality = lowLevelBufferHandler->getQualityCode(id, copy.get());
        dump.buffers.append(buffer);
    }
    return dump;
}

QVector<TimeStamp> SharedBufferReader::getTimestamps() const
{
    sharedMemory->lock();
    QVector<TimeStamp> timestamps = lowLevelBufferHandler->getTimestamps(sharedMemory->constData());
    sharedMemory->unlock();
    return timestamps;
}

SharedMemory::AccessMode SharedBufferReader::getAcessMode() const
{
    return SharedMemory::AccessMode::ReadOnly;
}
