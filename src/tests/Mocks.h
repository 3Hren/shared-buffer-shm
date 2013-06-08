#pragma once

#include "Global.h"

class LowLevelBufferHandlerMock : public LowLevelBufferHandler {
public:
    LowLevelBufferHandlerMock() : LowLevelBufferHandler(2, 4) {}
    MOCK_CONST_METHOD0(createStorage, void*());
    MOCK_CONST_METHOD3(push, void(TimeStamp, const SignalValue*, const void*));
    MOCK_CONST_METHOD1(getRawBuffersDump, char*(const void*));
    MOCK_CONST_METHOD2(getRawBuffer, SignalValue*(BufferId, const void*));
    MOCK_CONST_METHOD2(getBuffer, QVector<SignalValue>(BufferId, const void*));
    MOCK_CONST_METHOD2(getQualityCode, QualityCode(BufferId, const void*));
    MOCK_CONST_METHOD1(getTimestamps, QVector<TimeStamp>(const void*));
};

class SharedMemoryMock : public SharedMemory {
public:
    MOCK_METHOD2(create, bool(int, AccessMode));
    MOCK_CONST_METHOD0(size, int());
    MOCK_METHOD1(attach, bool(AccessMode));
    MOCK_CONST_METHOD0(isAttached, bool());

    MOCK_CONST_METHOD0(constData, const void*());
    MOCK_METHOD0(data, void*());

    MOCK_METHOD1(setKey, void(const QString&));
    MOCK_CONST_METHOD0(getKey, QString());

    MOCK_METHOD0(lock, bool());
    MOCK_METHOD0(unlock, bool());

    MOCK_CONST_METHOD0(getErrorDescription, QString());
};

class BeanFactoryMock : public BeanFactory {
public:
    SharedMemory *createSharedMemoryBean() const {
        return new SharedMemoryMock;
    }

    LowLevelBufferHandler *createLowLevelBufferHandler(BufferId buffersCount, BufferPos bufferSize) const {
        Q_UNUSED(buffersCount);
        Q_UNUSED(bufferSize);
        return new LowLevelBufferHandlerMock;
    }
};
