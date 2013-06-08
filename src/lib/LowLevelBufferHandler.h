#pragma once

#include "SharedBufferGlobal.h"
#include "domain/MetaData.h"

class LowLevelBufferHandler
{
    const BufferId buffersCount;
    const BufferPos bufferSize;
    const struct Internal * const internal;

public:
    LowLevelBufferHandler(BufferId buffersCount, BufferPos bufferSize);
    virtual ~LowLevelBufferHandler();

    BufferId getBuffersCount() const;
    BufferPos getBufferSize() const;

    quint32 getDataLengthBytes() const;
    quint32 getDumpLengthBytes() const;

    virtual void *createStorage() const;
    virtual void push(TimeStamp timestamp, const SignalValue *signalsPack, const void *data) const;
    virtual char *getBuffersDump(const void *data) const;
    virtual SignalValue *getRawBuffer(BufferId bufferId, const void *data) const;

    template<template<typename ...> class Container>
    Container<SignalValue> getBuffer(BufferId bufferId, const void *data) const {
        checkBufferId(bufferId);
        Container<SignalValue> values(bufferSize);
        parseBuffer(bufferId, data, values.data());
        return std::move(values);
    }

    virtual TimeStamp *getTimeStamps(const void *data) const;
    virtual QualityCode getQualityCode(BufferId bufferId, const void *data) const;
    virtual void setQualityCode(BufferId bufferId, QualityCode code, const void *data) const;

private:
    template<typename T>
    void reverse(char *data) const {
        T *tempArray = (T*)data;
        for (BufferId i = 0; i < bufferSize / 2; ++i) {
            T tmp = tempArray[i];
            tempArray[i] = tempArray[bufferSize - 1 - i];
            tempArray[bufferSize - 1 - i] = tmp;
        }
    }

    void parseBuffer(BufferId bufferId, const void *from, const SignalValue *to) const;
    void checkBufferId(BufferId bufferId) const;
};
