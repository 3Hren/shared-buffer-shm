#pragma once

#include <QString>

typedef quint32 BufferSize;
typedef quint16 BufferId;
typedef float SignalValue;
typedef quint64 TimeStamp;
typedef quint16 ErrorCode;

static_assert(sizeof(BufferSize) == sizeof(SignalValue), "sizeof(BufferSize) must be equal sizeof(SignalValue) because of memcopy operations");
