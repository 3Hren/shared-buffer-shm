#pragma once

#include <QVector>
#include "SharedBufferGlobal.h"

struct Buffer {
    QVector<SignalValue> values;
    QualityCode quality;

    inline bool operator ==(const Buffer &other) const {
        return values == other.values && quality == other.quality;
    }
};

struct BuffersDump {
    QVector<TimeStamp> timestamps;
    QVector<Buffer> buffers;

    inline bool operator ==(const BuffersDump &other) const {
        return timestamps == other.timestamps && buffers == other.buffers;
    }
};
