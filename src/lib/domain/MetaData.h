#pragma once

#include "SharedBufferGlobal.h"
#include <cstring>

struct MetaDataHeader {
    BufferPos currentPos = 0;
    quint32 clientCount = 0;
    BufferPos bufferSize = 0;
    BufferId buffersCount = 0;
};

template<uint placeholderSize>
struct MetaDataStruct : public MetaDataHeader {
    char placeholder[placeholderSize];

    MetaDataStruct() {
        std::memset(placeholder, 0, placeholderSize);
    }
};

template<>
struct MetaDataStruct<0> : public MetaDataHeader {};

struct MetaData : MetaDataStruct<112> {};
static_assert(sizeof(MetaData) == 128, "sizeof(MetaData) must be equal 128 for language frameworks compatibility");
