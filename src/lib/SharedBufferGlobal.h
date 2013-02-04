#pragma once

#include <QString>

typedef quint32 BufferPos;
typedef quint16 BufferId;
typedef float SignalValue;
typedef quint64 TimeStamp;
typedef quint16 ValidityCode;

static_assert(sizeof(BufferPos) == sizeof(SignalValue), "sizeof(BufferPos) must be equal sizeof(SignalValue) because of memcopy operations");
