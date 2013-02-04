#include "Global.h"
#include "LowLevelBufferManager.h"

#include <boost/shared_array.hpp>
#include <boost/scoped_array.hpp>

TEST(LowLevelBufferManager, InitializationConstructor) {
    LowLevelBufferManager manager(64, 1024);
    EXPECT_EQ(64, manager.getBuffersCount());
    EXPECT_EQ(1024, manager.getBufferSize());
}

char *createStorage(BufferId BUFFERS_COUNT, BufferPos BUFFER_SIZE, BufferPos pos, SignalValue buffers[], ValidityCode codes[], TimeStamp timeStamps[], int length) {
    char *storage = new char[length];
    memset(storage, 0, length);
    memcpy(storage, &pos, sizeof(BufferPos));
    memcpy(storage + sizeof(BufferPos), buffers, (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue));
    memcpy(storage + sizeof(BufferPos) + BUFFERS_COUNT * BUFFER_SIZE * sizeof(SignalValue), codes, BUFFERS_COUNT * sizeof(ValidityCode));
    memcpy(storage + sizeof(BufferPos) + BUFFERS_COUNT * BUFFER_SIZE * sizeof(SignalValue) + BUFFERS_COUNT * sizeof(ValidityCode), timeStamps, BUFFER_SIZE * sizeof(TimeStamp));
    return storage;
}

TEST(LowLevelBufferManager, Push) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferPos BUFFER_SIZE = 4;
    LowLevelBufferManager manager(BUFFERS_COUNT, BUFFER_SIZE);

    BufferPos expectedCurrentPos = 0;
    SignalValue expectedBuffers[] = {
        0, 0, 0, 0,
        1, 0, 0, 0,
        2, 0, 0, 0,
        3, 0, 0, 0,
        4, 0, 0, 0,
        5, 0, 0, 0,
        6, 0, 0, 0,
        7, 0, 0, 0,
        8, 0, 0, 0,
        9, 0, 0, 0
    };
    ValidityCode expectedValidityCodes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    TimeStamp expectedTimeStamps[] = {10, 0, 0, 0};
    const int length = manager.getDataLength();
    boost::scoped_array<char> expected(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                     expectedCurrentPos,
                                                     expectedBuffers,
                                                     expectedValidityCodes,
                                                     expectedTimeStamps,
                                                     length));

    boost::scoped_array<SignalValue> signalsPackToPush(new SignalValue[BUFFERS_COUNT]);
    for (int i = 0; i < BUFFERS_COUNT; ++i)
        signalsPackToPush[i] = i;

    void *storage = manager.createStorage();
    manager.push(10, signalsPackToPush.get(), storage);

    EXPECT_TRUE(0 == std::memcmp(expected.get(), storage, length));

    delete[] (char*)storage;
}

TEST(LowLevelBufferManager, MultiplePush) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferPos BUFFER_SIZE = 4;

    LowLevelBufferManager manager(BUFFERS_COUNT, BUFFER_SIZE);
    void *storage = manager.createStorage();
    for (int j = 0; j < 3; ++j) {
        boost::scoped_array<SignalValue> signalsPack(new SignalValue[BUFFERS_COUNT]);
        for (int i = 0; i < BUFFERS_COUNT; ++i)
            signalsPack[i] = i;
        manager.push(j, signalsPack.get(), (void*)storage);
    }

    BufferPos expectedCurrentPos = 2;
    SignalValue expectedBuffers[] = {
        0, 0, 0, 0,
        1, 1, 1, 0,
        2, 2, 2, 0,
        3, 3, 3, 0,
        4, 4, 4, 0,
        5, 5, 5, 0,
        6, 6, 6, 0,
        7, 7, 7, 0,
        8, 8, 8, 0,
        9, 9, 9, 0
    };
    ValidityCode expectedValidityCodes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    TimeStamp expectedTimeStamps[] = {0, 1, 2, 0};

    const int length = manager.getDataLength();
    boost::scoped_array<char> expected(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                     expectedCurrentPos,
                                                     expectedBuffers,
                                                     expectedValidityCodes,
                                                     expectedTimeStamps,
                                                     length));
    EXPECT_TRUE(0 == std::memcmp(expected.get(), storage, length));

    delete[] (char*)storage;
}

TEST(LowLevelBufferManager, MultiplePushWithFullingBuffer) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferPos BUFFER_SIZE = 4;

    LowLevelBufferManager manager(BUFFERS_COUNT, BUFFER_SIZE);
    void *storage = manager.createStorage();
    for (int j = 0; j < 4; ++j) {
        boost::scoped_array<SignalValue> signalsPack(new SignalValue[BUFFERS_COUNT]);
        for (int i = 0; i < BUFFERS_COUNT; ++i)
            signalsPack[i] = j;
        manager.push(j, signalsPack.get(), (void*)storage);
    }

    BufferPos expectedCurrentPos = 3;
    SignalValue expectedBuffers[] = {
        0, 1, 2, 3,
        0, 1, 2, 3,
        0, 1, 2, 3,
        0, 1, 2, 3,
        0, 1, 2, 3,
        0, 1, 2, 3,
        0, 1, 2, 3,
        0, 1, 2, 3,
        0, 1, 2, 3,
        0, 1, 2, 3
    };
    ValidityCode expectedValidityCodes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    TimeStamp expectedTimeStamps[] = {0, 1, 2, 3};

    const int length = manager.getDataLength();
    boost::scoped_array<char> expected(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                     expectedCurrentPos,
                                                     expectedBuffers,
                                                     expectedValidityCodes,
                                                     expectedTimeStamps,
                                                     length));

    EXPECT_TRUE(0 == std::memcmp(expected.get(), storage, length));

    delete[] (char*)storage;
}

TEST(LowLevelBufferManager, MultiplePushWithOverriding) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferPos BUFFER_SIZE = 4;

    LowLevelBufferManager manager(BUFFERS_COUNT, BUFFER_SIZE);
    void *storage = manager.createStorage();
    for (int j = 0; j < 5; ++j) {
        boost::scoped_array<SignalValue> signalsPack(new SignalValue[BUFFERS_COUNT]);
        for (int i = 0; i < BUFFERS_COUNT; ++i)
            signalsPack[i] = j;
        manager.push(j, signalsPack.get(), (void*)storage);
    }

    BufferPos expectedCurrentPos = 0;
    SignalValue expectedBuffers[] = {
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3
    };
    ValidityCode expectedValidityCodes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    TimeStamp expectedTimeStamps[] = {4, 1, 2, 3};
    const int length = manager.getDataLength();
    boost::scoped_array<char> expected(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                     expectedCurrentPos,
                                                     expectedBuffers,
                                                     expectedValidityCodes,
                                                     expectedTimeStamps,
                                                     length));
    EXPECT_TRUE(0 == std::memcmp(expected.get(), storage, length));

    delete[] (char*)storage;
}

TEST(LowLevelBufferManager, GetBuffersDump) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferPos BUFFER_SIZE = 4;
    LowLevelBufferManager manager(BUFFERS_COUNT, BUFFER_SIZE);

    BufferPos currentPos = 0;
    SignalValue buffersDump[] = {
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3
    };
    ValidityCode validityCodesDump[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TimeStamp timeStampsDump[] = {4, 1, 2, 3};

    const int length = manager.getDataLength();
    boost::scoped_array<char> storage(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                    currentPos,
                                                    buffersDump,
                                                    validityCodesDump,
                                                    timeStampsDump,
                                                    length));

    const int dumpLength = manager.getDumpLength();
    char *expected = new char[dumpLength];
    memset(expected, 0, dumpLength);
    SignalValue expectedBufferDump[] = {
        4, 3, 2, 1,
        4, 3, 2, 1,
        4, 3, 2, 1,
        4, 3, 2, 1,
        4, 3, 2, 1,
        4, 3, 2, 1,
        4, 3, 2, 1,
        4, 3, 2, 1,
        4, 3, 2, 1,
        4, 3, 2, 1
    };
    ValidityCode expectedValidityCodes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TimeStamp expectedTimeStamps[] = {4, 3, 2, 1};
    memcpy(expected, expectedBufferDump, (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue));
    memcpy(expected + (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue), expectedValidityCodes, BUFFERS_COUNT * sizeof(ValidityCode));
    memcpy(expected + (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue) + BUFFERS_COUNT * sizeof(ValidityCode), expectedTimeStamps, BUFFER_SIZE * sizeof(TimeStamp));

    char *dump = manager.getBuffersDump((void*)storage.get());
    EXPECT_TRUE(0 == std::memcmp(expected, dump, dumpLength));

    //! Cleanup
    delete[] expected;
    delete[] dump;
}

#include "HighLevelBufferManager.h"
TEST(HighLevelBufferManager, Class) {
    LowLevelBufferManager manager(10, 20);
    HighLevelBufferManager decorator(&manager);
    Q_UNUSED(decorator);
}

//! @note: Пригодится

//for (int i = 0; i < length; ++i)
//    qDebug() << i << (float)(expected.get())[i] << (float)((char*)storage)[i];
