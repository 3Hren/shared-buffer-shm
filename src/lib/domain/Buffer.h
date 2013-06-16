#pragma once

#include <QVector>

#include "config.hpp"
#include "SharedBufferGlobal.h"

template<template<typename...> class Vector>
struct TypedBuffer {
    Vector<SignalValue> values;
    QualityCode quality;

    inline bool operator ==(const TypedBuffer<Vector> &other) const {
        return values == other.values && quality == other.quality;
    }
};

template<template<typename...> class Vector>
struct TypedBuffersDump {
    Vector<TimeStamp> timestamps;
    Vector<TypedBuffer<Vector>> buffers;

    inline bool operator ==(const TypedBuffersDump &other) const {
        return timestamps == other.timestamps && buffers == other.buffers;
    }
};

typedef TypedBuffer<QVector> Buffer;
typedef TypedBuffersDump<QVector> BuffersDump;

#ifdef SHARBUF_DEBUG
#include <QDebug>
#include <QStringList>
inline QDebug operator <<(QDebug debug, const Buffer &buffer) {
    QStringList values;
    for(auto value : buffer.values)
        values << QString::number(value);
    debug << QString("Buffer(%1 : %2)").arg(values.join(", ")).arg(buffer.quality).toLatin1().constData();
    return debug;
}
#endif
