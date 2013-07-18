#pragma once

#include "AbstractSharedBufferHandler.h"

class SharedBufferReader : public AbstractSharedBufferHandler
{
public:
    template<template<typename...> class Vector>
    TypedBuffer<Vector> getBuffer(BufferId bufferId) const;

    template<template<typename...> class Vector>
    TypedBuffer<Vector> getBuffer(BufferId bufferId, BufferId size) const;

    BuffersDump getBuffersDump() const;
    QVector<TimeStamp> getTimestamps() const;
    QualityCode getQualityCode(BufferId bufferId) const;

protected:
    AccessMode getAcessMode() const override final;
};
