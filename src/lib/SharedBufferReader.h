#pragma once

#include "AbstractSharedBufferHandler.h"
#include "domain/Buffer.h"

#include "LowLevelBufferHandler.h"

class SharedBufferReader : public AbstractSharedBufferHandler
{    
public:
    template<template<typename...> class Container>
    TypedBuffer<Container> getBuffer(BufferId bufferId) const {
        sharedMemory->lock();
        const void *data = sharedMemory->constData();
        Container<SignalValue> values = lowLevelBufferHandler->getBuffer<Container>(bufferId, data);
        QualityCode quality = lowLevelBufferHandler->getQualityCode(bufferId, data);
        sharedMemory->unlock();
        const TypedBuffer<Container> buffer{values, quality};
        return buffer;
    }

    Buffer getBuffer(BufferId bufferId) const;
    BuffersDump getBuffersDump() const;
    QVector<TimeStamp> getTimestamps() const;
    QualityCode getQualityCode(BufferId bufferId) const;    

protected:
    SharedMemory::AccessMode getAcessMode() const override final;
};
