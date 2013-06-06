#include "Global.h"
#include "LowLevelBufferHandler.h"

#include <boost/shared_array.hpp>
#include <boost/scoped_array.hpp>
#include <cstring>

TEST(LowLevelBufferHandler, InitializationConstructor) {
    LowLevelBufferHandler manager(64, 1024);
    EXPECT_EQ(64, manager.getBuffersCount());
    EXPECT_EQ(1024, manager.getBufferSize());
}

char *createStorage(BufferId BUFFERS_COUNT, BufferPos BUFFER_SIZE, BufferPos pos, SignalValue buffers[], ValidityCode codes[], TimeStamp timeStamps[], int length) {
    char *storage = new char[length];
    MetaData meta;
    meta.currentPos = pos;
    memset(storage, 0, length);

    memcpy(storage, &meta, sizeof(MetaData));
    memcpy(storage + sizeof(MetaData), buffers, (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue));
    memcpy(storage + sizeof(MetaData) + BUFFERS_COUNT * BUFFER_SIZE * sizeof(SignalValue), codes, BUFFERS_COUNT * sizeof(ValidityCode));
    memcpy(storage + sizeof(MetaData) + BUFFERS_COUNT * BUFFER_SIZE * sizeof(SignalValue) + BUFFERS_COUNT * sizeof(ValidityCode), timeStamps, BUFFER_SIZE * sizeof(TimeStamp));
    return storage;
}

TEST(LowLevelBufferHandler, Push) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferPos BUFFER_SIZE = 4;
    LowLevelBufferHandler manager(BUFFERS_COUNT, BUFFER_SIZE);

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
    TimeStamp expectedTimeStamps[] = {1, 0, 0, 0};
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
    manager.push(1, signalsPackToPush.get(), storage);

    EXPECT_TRUE(0 == std::memcmp(expected.get(), storage, length));

    delete[] (char*)storage;
}

TEST(LowLevelBufferHandler, MultiplePush) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferPos BUFFER_SIZE = 4;

    LowLevelBufferHandler manager(BUFFERS_COUNT, BUFFER_SIZE);
    void *storage = manager.createStorage();
    for (int j = 0; j < 3; ++j) {
        boost::scoped_array<SignalValue> signalsPack(new SignalValue[BUFFERS_COUNT]);
        for (int i = 0; i < BUFFERS_COUNT; ++i)
            signalsPack[i] = i;
        manager.push(j, signalsPack.get(), storage);
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

TEST(LowLevelBufferHandler, MultiplePushWithFullingBuffer) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferPos BUFFER_SIZE = 4;

    LowLevelBufferHandler manager(BUFFERS_COUNT, BUFFER_SIZE);
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

TEST(LowLevelBufferHandler, MultiplePushWithOverriding) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferPos BUFFER_SIZE = 4;

    LowLevelBufferHandler manager(BUFFERS_COUNT, BUFFER_SIZE);
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

TEST(LowLevelBufferHandler, GetBuffersDump) {
    const BufferId BUFFERS_COUNT = 10;
    const BufferPos BUFFER_SIZE = 4;
    LowLevelBufferHandler manager(BUFFERS_COUNT, BUFFER_SIZE);

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

TEST(LowLevelBufferHandler, GetBuffer) {
    const BufferId BUFFERS_COUNT = 10;
    const BufferPos BUFFER_SIZE = 4;
    LowLevelBufferHandler manager(BUFFERS_COUNT, BUFFER_SIZE);

    BufferPos currentPos = 0;
    SignalValue buffersDump[] = {
        4, 1, 2, 3,
        8, 5, 6, 7,
        0, 9, 0, 0,
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
    SignalValue expected1[] = {4, 3, 2, 1};
    std::unique_ptr<SignalValue[]> actual1(manager.getBuffer(0, (void*)storage.get()));
    EXPECT_TRUE(0 == std::memcmp(expected1, actual1.get(), 4 * sizeof(SignalValue)));

    SignalValue expected2[] = {8, 7, 6, 5};
    std::unique_ptr<SignalValue[]> actual2(manager.getBuffer(1, (void*)storage.get()));
    EXPECT_TRUE(0 == std::memcmp(expected2, actual2.get(), 4 * sizeof(SignalValue)));

    SignalValue expected3[] = {0, 0, 0, 9};
    std::unique_ptr<SignalValue[]> actual3(manager.getBuffer(2, (void*)storage.get()));
    EXPECT_TRUE(0 == std::memcmp(expected3, actual3.get(), 4 * sizeof(SignalValue)));
}

TEST(LowLevelBufferHandler, ParseTimestamps) {
    const BufferId BUFFERS_COUNT = 10;
    const BufferPos BUFFER_SIZE = 4;
    LowLevelBufferHandler manager(BUFFERS_COUNT, BUFFER_SIZE);

    BufferPos currentPos = 0;
    SignalValue buffersDump[] = {
        4, 1, 2, 3,
        8, 5, 6, 7,
        0, 9, 0, 0,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3
    };
    ValidityCode validityCodesDump[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TimeStamp timeStampsDump[] = {9, 6, 7, 8};

    const int length = manager.getDataLength();
    boost::scoped_array<char> storage(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                    currentPos,
                                                    buffersDump,
                                                    validityCodesDump,
                                                    timeStampsDump,
                                                    length));
    TimeStamp expected[] = {9, 8, 7, 6};
    std::unique_ptr<TimeStamp[]> actual(manager.getTimeStamps((void*)storage.get()));
    EXPECT_TRUE(0 == std::memcmp(expected, actual.get(), 4 * sizeof(TimeStamp)));
}

TEST(LowLevelBufferHandler, ParseValidityCode) {
    const BufferId BUFFERS_COUNT = 10;
    const BufferPos BUFFER_SIZE = 4;
    LowLevelBufferHandler manager(BUFFERS_COUNT, BUFFER_SIZE);

    BufferPos currentPos = 0;
    SignalValue buffersDump[] = {
        4, 1, 2, 3,
        8, 5, 6, 7,
        0, 9, 0, 0,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3,
        4, 1, 2, 3
    };
    ValidityCode validityCodesDump[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TimeStamp timeStampsDump[] = {9, 6, 7, 8};

    const int length = manager.getDataLength();
    boost::scoped_array<char> storage(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                    currentPos,
                                                    buffersDump,
                                                    validityCodesDump,
                                                    timeStampsDump,
                                                    length));
    EXPECT_EQ(0, manager.getValidityCode(0, storage.get()));
    EXPECT_EQ(1, manager.getValidityCode(1, storage.get()));
    EXPECT_EQ(2, manager.getValidityCode(2, storage.get()));
    EXPECT_EQ(3, manager.getValidityCode(3, storage.get()));
    EXPECT_EQ(4, manager.getValidityCode(4, storage.get()));
    EXPECT_EQ(5, manager.getValidityCode(5, storage.get()));
    EXPECT_EQ(6, manager.getValidityCode(6, storage.get()));
    EXPECT_EQ(7, manager.getValidityCode(7, storage.get()));
    EXPECT_EQ(8, manager.getValidityCode(8, storage.get()));
    EXPECT_EQ(9, manager.getValidityCode(9, storage.get()));
}

//! @note: Пригодится

//for (int i = 0; i < length; ++i)
//    qDebug() << i << (float)(expected.get())[i] << (float)((char*)storage)[i];
//for (int i = 0; i < BUFFERS_COUNT * BUFFER_SIZE; ++i)
//    qDebug() << i << ((SignalValue*)expected)[i] << ((SignalValue*)dump)[i];
