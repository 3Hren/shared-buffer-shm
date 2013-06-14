#pragma once

#include "SharedBufferGlobal.h"

struct Internal {
    const quint16 META_DATA_SIZE_BYTES;
    const quint32 BUFFER_DATA_SIZE_BYTES;
    const quint32 BUFFERS_DATA_SIZE_BYTES;
    const quint32 QUALITY_DATA_SIZE_BYTES;
    const quint32 TIMESTAMP_DATA_SIZE_BYTES;

    const quint32 TIMESTAMPS_OFFSET;

    constexpr Internal(BufferPos size, BufferId count) :
        META_DATA_SIZE_BYTES(sizeof(MetaData)),
        BUFFER_DATA_SIZE_BYTES(size * sizeof(SignalValue)),
        BUFFERS_DATA_SIZE_BYTES(count * size * sizeof(SignalValue)),
        QUALITY_DATA_SIZE_BYTES(count * sizeof(QualityCode)),
        TIMESTAMP_DATA_SIZE_BYTES(size * sizeof(TimeStamp)),

        TIMESTAMPS_OFFSET(sizeof(MetaData) + count * size * sizeof(SignalValue) + count * sizeof(QualityCode))
    {}

    inline constexpr quint32 getDataLengthBytes() const {
        return META_DATA_SIZE_BYTES + BUFFERS_DATA_SIZE_BYTES + QUALITY_DATA_SIZE_BYTES + TIMESTAMP_DATA_SIZE_BYTES;
    }

    inline constexpr quint32 getDumpLengthBytes() const {
        return getDataLengthBytes() - META_DATA_SIZE_BYTES;
    }
};
