#pragma once

#include "SharedBufferGlobal.h"
#include "domain/MetaData.h"
//#include <QVector>//! @todo: Не нравится мне тут зависимость от Qt

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
    virtual void push(TimeStamp timestamp, const SignalValue *signalsPack, void *data) const;
    virtual char *getRawBuffersDump(const void *data) const;
    virtual SignalValue *getRawBuffer(BufferId bufferId, const void *data) const;
    virtual TimeStamp *getRawTimeStamps(const void *data) const;

    template<template<typename ...> class Vector>
    Vector<SignalValue> getBuffer(BufferId bufferId, const void *data) const {
        return getBuffer<Vector>(bufferId, bufferSize, data);
    }
    virtual QVector<SignalValue> getBuffer(BufferId bufferId, const void *data) const;

    template<template<typename ...> class Vector>
    Vector<SignalValue> getBuffer(BufferId bufferId, BufferId count, const void *data) const {
        checkBufferId(bufferId);
        checkSizeRequested(count);
        Vector<SignalValue> values(count);
        parseBuffer(bufferId, count, data, values.data());
        return std::move(values);
    }

    template<template<typename ...> class Vector>
    Vector<TimeStamp> getTimestamps(const void *data) const {
        Vector<TimeStamp> timestamps(bufferSize);
        parseTimestamps(data, timestamps.data());
        return timestamps;
    }
    virtual QVector<TimeStamp> getTimestamps(const void *data) const;

    virtual QualityCode getQualityCode(BufferId bufferId, const void *data) const;
    virtual void setQualityCode(BufferId bufferId, QualityCode code, void *data) const;

private:
    template<typename T>
    void reverse(char *data) const {
        reverse<T>(data, bufferSize);
    }

    template<typename T>
    void reverse(char *data, BufferId size) const {
        T *tempArray = reinterpret_cast<T*>(data);
        for (BufferPos i = 0; i < size / 2; ++i)
            std::swap(tempArray[i], tempArray[size - 1 - i]);
    }

    void parseBuffer(BufferId bufferId, const void *from, SignalValue *to) const;
    void parseBuffer(BufferId bufferId, BufferId count, const void *from, SignalValue *to) const;
    void parseTimestamps(const void *from, TimeStamp *to) const;
    void checkBufferId(BufferId bufferId) const;
    void checkSizeRequested(BufferId size) const;
};
