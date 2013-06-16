#pragma once

#include <QString>

typedef quint32 BufferPos;
typedef quint32 BufferId;
typedef float SignalValue;
typedef quint64 TimeStamp;
typedef quint16 QualityCode;

static_assert(static_cast<BufferId>(-1) > 0, "BufferId must be unsigned");
static_assert(sizeof(BufferPos) == sizeof(SignalValue), "sizeof(BufferPos) must be equal sizeof(SignalValue) because of memcopy operations");
