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

#ifdef DEBUG
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
