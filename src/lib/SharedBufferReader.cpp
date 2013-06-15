#include "SharedBufferReader.h"

#include <memory>
#include <cstring>

#include <QDebug>

#include "LowLevelBufferHandler.h"
#include "SharedMemoryLocker.h"

template<template<typename...> class Vector>
TypedBuffer<Vector> SharedBufferReader::getBuffer(BufferId bufferId) const
{
    return getBuffer<Vector>(bufferId, lowLevelBufferHandler->getBufferSize());
}

template<template<typename...> class Vector>
TypedBuffer<Vector> SharedBufferReader::getBuffer(BufferId bufferId, BufferId size) const
{
    SharedMemoryLocker<SharedMemory> locker(sharedMemory);
    const void *data = sharedMemory->constData();
    Vector<SignalValue> values = lowLevelBufferHandler->getBuffer<Vector>(bufferId, size, data);
    QualityCode quality = lowLevelBufferHandler->getQualityCode(bufferId, data);
    locker.unlock();
    const TypedBuffer<Vector> buffer{values, quality};
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

QualityCode SharedBufferReader::getQualityCode(BufferId bufferId) const
{
    sharedMemory->lock();
    QualityCode qualityCode = lowLevelBufferHandler->getQualityCode(bufferId, sharedMemory->constData());
    sharedMemory->unlock();
    return qualityCode;
}

SharedMemory::AccessMode SharedBufferReader::getAcessMode() const
{
    return SharedMemory::AccessMode::ReadOnly;
}

template TypedBuffer<QVector> SharedBufferReader::getBuffer<QVector>(BufferId bufferId) const;
template TypedBuffer<QVector> SharedBufferReader::getBuffer<QVector>(BufferId bufferId, BufferId size) const;

template TypedBuffer<std::vector> SharedBufferReader::getBuffer<std::vector>(BufferId bufferId) const;
template TypedBuffer<std::vector> SharedBufferReader::getBuffer<std::vector>(BufferId bufferId, BufferId size) const;
