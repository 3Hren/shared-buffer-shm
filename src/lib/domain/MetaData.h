#pragma once

#include "SharedBufferGlobal.h"
#include <cstring>

template<uint placeholderSize>
struct MetaDataStruct {
    BufferPos currentPos;
    char placeholder[placeholderSize];

    MetaDataStruct() : currentPos(0) {
        std::memset(placeholder, 0, placeholderSize);
    }
};

template<>
struct MetaDataStruct<0> {
    BufferPos currentPos;
    MetaDataStruct() : currentPos(0) {}
};

struct MetaData : MetaDataStruct<124> {};
static_assert(sizeof(MetaData) == 128, "sizeof(MetaData) must be equal 128 for language frameworks compatibility");
