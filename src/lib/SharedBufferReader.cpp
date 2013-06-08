#include "SharedBufferReader.h"

#include "LowLevelBufferHandler.h"

#include <memory>

Buffer SharedBufferReader::getBuffer(BufferId bufferId) const
{
    sharedMemory->lock();
    const void *data = sharedMemory->constData();
    QVector<SignalValue> values = lowLevelBufferHandler->getBuffer<QVector>(bufferId, data);
    QualityCode quality = lowLevelBufferHandler->getQualityCode(bufferId, data);
    sharedMemory->unlock();
    const Buffer buffer{values, quality};
    return buffer;
}

SharedMemory::AccessMode SharedBufferReader::getAcessMode() const
{
    return SharedMemory::AccessMode::ReadOnly;
}
