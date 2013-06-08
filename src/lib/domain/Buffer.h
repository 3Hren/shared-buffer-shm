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
