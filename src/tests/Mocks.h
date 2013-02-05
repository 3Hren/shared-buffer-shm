#pragma once

#include "Global.h"

class LowLevelBufferHandlerMock : public LowLevelBufferHandler {
public:
    LowLevelBufferHandlerMock() : LowLevelBufferHandler(0, 0) {}
    MOCK_CONST_METHOD0(createStorage, void*());
    MOCK_CONST_METHOD3(push, void(TimeStamp, const SignalValue*, const void*));
    MOCK_CONST_METHOD1(getBuffersDump, char*(const void*));
};
