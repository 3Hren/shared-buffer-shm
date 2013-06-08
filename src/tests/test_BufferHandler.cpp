#include "Global.h"
#include "LowLevelBufferHandler.h"

#include <boost/shared_array.hpp>
#include <boost/scoped_array.hpp>
#include <cstring>
#include <stdexcept>

TEST(LowLevelBufferHandler, InitializationConstructor) {
    LowLevelBufferHandler manager(64, 1024);
    EXPECT_EQ(64, manager.getBuffersCount());
    EXPECT_EQ(1024, manager.getBufferSize());
}

char *createStorage(BufferId BUFFERS_COUNT, BufferPos BUFFER_SIZE, BufferPos pos, SignalValue buffers[], QualityCode codes[], TimeStamp timeStamps[], int length) {
    char *storage = new char[length];
    MetaData meta;
    meta.currentPos = pos;
    memset(storage, 0, length);

    memcpy(storage, &meta, sizeof(MetaData));
    memcpy(storage + sizeof(MetaData), buffers, (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue));
    memcpy(storage + sizeof(MetaData) + BUFFERS_COUNT * BUFFER_SIZE * sizeof(SignalValue), codes, BUFFERS_COUNT * sizeof(QualityCode));
    memcpy(storage + sizeof(MetaData) + BUFFERS_COUNT * BUFFER_SIZE * sizeof(SignalValue) + BUFFERS_COUNT * sizeof(QualityCode), timeStamps, BUFFER_SIZE * sizeof(TimeStamp));
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
    QualityCode expectedQualityCodes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    TimeStamp expectedTimeStamps[] = {1, 0, 0, 0};
    const int length = manager.getDataLengthBytes();
    boost::scoped_array<char> expected(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                     expectedCurrentPos,
                                                     expectedBuffers,
                                                     expectedQualityCodes,
                                                     expectedTimeStamps,
                                                     length));

    boost::scoped_array<SignalValue> signalsPackToPush(new SignalValue[BUFFERS_COUNT]);
    for (BufferId i = 0; i < BUFFERS_COUNT; ++i)
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
        for (BufferId i = 0; i < BUFFERS_COUNT; ++i)
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
    QualityCode expectedQualityCodes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    TimeStamp expectedTimeStamps[] = {0, 1, 2, 0};

    const int length = manager.getDataLengthBytes();
    boost::scoped_array<char> expected(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                     expectedCurrentPos,
                                                     expectedBuffers,
                                                     expectedQualityCodes,
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
        for (BufferId i = 0; i < BUFFERS_COUNT; ++i)
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
    QualityCode expectedQualityCodes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    TimeStamp expectedTimeStamps[] = {0, 1, 2, 3};

    const int length = manager.getDataLengthBytes();
    boost::scoped_array<char> expected(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                     expectedCurrentPos,
                                                     expectedBuffers,
                                                     expectedQualityCodes,
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
        for (BufferId i = 0; i < BUFFERS_COUNT; ++i)
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
    QualityCode expectedQualityCodes[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    TimeStamp expectedTimeStamps[] = {4, 1, 2, 3};
    const int length = manager.getDataLengthBytes();
    boost::scoped_array<char> expected(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                     expectedCurrentPos,
                                                     expectedBuffers,
                                                     expectedQualityCodes,
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
    QualityCode qualityCodesDump[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TimeStamp timeStampsDump[] = {4, 1, 2, 3};

    const int length = manager.getDataLengthBytes();
    boost::scoped_array<char> storage(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                    currentPos,
                                                    buffersDump,
                                                    qualityCodesDump,
                                                    timeStampsDump,
                                                    length));

    const int dumpLength = manager.getDumpLengthBytes();
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
    QualityCode expectedQualityCodes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TimeStamp expectedTimeStamps[] = {4, 3, 2, 1};
    memcpy(expected, expectedBufferDump, (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue));
    memcpy(expected + (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue), expectedQualityCodes, BUFFERS_COUNT * sizeof(QualityCode));
    memcpy(expected + (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue) + BUFFERS_COUNT * sizeof(QualityCode), expectedTimeStamps, BUFFER_SIZE * sizeof(TimeStamp));

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
    QualityCode qualityCodesDump[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TimeStamp timeStampsDump[] = {4, 1, 2, 3};

    const int length = manager.getDataLengthBytes();
    boost::scoped_array<char> storage(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                    currentPos,
                                                    buffersDump,
                                                    qualityCodesDump,
                                                    timeStampsDump,
                                                    length));
    SignalValue expected1[] = {4, 3, 2, 1};
    std::unique_ptr<SignalValue[]> actual1(manager.getRawBuffer(0, (void*)storage.get()));
    EXPECT_TRUE(0 == std::memcmp(expected1, actual1.get(), 4 * sizeof(SignalValue)));

    SignalValue expected2[] = {8, 7, 6, 5};
    std::unique_ptr<SignalValue[]> actual2(manager.getRawBuffer(1, (void*)storage.get()));
    EXPECT_TRUE(0 == std::memcmp(expected2, actual2.get(), 4 * sizeof(SignalValue)));

    SignalValue expected3[] = {0, 0, 0, 9};
    std::unique_ptr<SignalValue[]> actual3(manager.getRawBuffer(2, (void*)storage.get()));
    EXPECT_TRUE(0 == std::memcmp(expected3, actual3.get(), 4 * sizeof(SignalValue)));
}

TEST(LowLevelBufferHandler, GetBufferToVector) {
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
    QualityCode qualityCodesDump[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TimeStamp timeStampsDump[] = {4, 1, 2, 3};

    const int length = manager.getDataLengthBytes();
    boost::scoped_array<char> storage(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                    currentPos,
                                                    buffersDump,
                                                    qualityCodesDump,
                                                    timeStampsDump,
                                                    length));
    EXPECT_EQ(QVector<SignalValue>({4, 3, 2, 1}), manager.getBuffer<QVector>(0, (void*)storage.get()));
    EXPECT_EQ(QVector<SignalValue>({8, 7, 6, 5}), manager.getBuffer<QVector>(1, (void*)storage.get()));
    EXPECT_EQ(QVector<SignalValue>({0, 0, 0, 9}), manager.getBuffer<QVector>(2, (void*)storage.get()));
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
    QualityCode qualityCodesDump[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TimeStamp timeStampsDump[] = {9, 6, 7, 8};

    const int length = manager.getDataLengthBytes();
    boost::scoped_array<char> storage(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                    currentPos,
                                                    buffersDump,
                                                    qualityCodesDump,
                                                    timeStampsDump,
                                                    length));
    TimeStamp expected[] = {9, 8, 7, 6};
    std::unique_ptr<TimeStamp[]> actual(manager.getRawTimeStamps((void*)storage.get()));
    EXPECT_TRUE(0 == std::memcmp(expected, actual.get(), 4 * sizeof(TimeStamp)));
}

TEST(LowLevelBufferHandler, ParseTimestampsToVector) {
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
    QualityCode qualityCodesDump[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TimeStamp timeStampsDump[] = {9, 6, 7, 8};

    const int length = manager.getDataLengthBytes();
    boost::scoped_array<char> storage(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                    currentPos,
                                                    buffersDump,
                                                    qualityCodesDump,
                                                    timeStampsDump,
                                                    length));
    QVector<TimeStamp> expected = {9, 8, 7, 6};
    EXPECT_EQ(expected, manager.getTimestamps((void*)storage.get()));
}

TEST(LowLevelBufferHandler, ParseQualityCode) {
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
    QualityCode qualityCodesDump[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TimeStamp timeStampsDump[] = {9, 6, 7, 8};

    const int length = manager.getDataLengthBytes();
    boost::scoped_array<char> storage(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                    currentPos,
                                                    buffersDump,
                                                    qualityCodesDump,
                                                    timeStampsDump,
                                                    length));
    EXPECT_EQ(0, manager.getQualityCode(0, storage.get()));
    EXPECT_EQ(1, manager.getQualityCode(1, storage.get()));
    EXPECT_EQ(2, manager.getQualityCode(2, storage.get()));
    EXPECT_EQ(3, manager.getQualityCode(3, storage.get()));
    EXPECT_EQ(4, manager.getQualityCode(4, storage.get()));
    EXPECT_EQ(5, manager.getQualityCode(5, storage.get()));
    EXPECT_EQ(6, manager.getQualityCode(6, storage.get()));
    EXPECT_EQ(7, manager.getQualityCode(7, storage.get()));
    EXPECT_EQ(8, manager.getQualityCode(8, storage.get()));
    EXPECT_EQ(9, manager.getQualityCode(9, storage.get()));
}

TEST(LowLevelBufferHandler, SetQualityCode) {
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
    QualityCode qualityCodesDump[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    TimeStamp timeStampsDump[] = {9, 6, 7, 8};

    const int length = manager.getDataLengthBytes();
    boost::scoped_array<char> storage(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                    currentPos,
                                                    buffersDump,
                                                    qualityCodesDump,
                                                    timeStampsDump,
                                                    length));

    QualityCode expectedQualty[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    boost::scoped_array<char> expected(createStorage(BUFFERS_COUNT, BUFFER_SIZE,
                                                     currentPos,
                                                     buffersDump,
                                                     expectedQualty,
                                                     timeStampsDump,
                                                     length));
    manager.setQualityCode(0, 0, storage.get());
    manager.setQualityCode(1, 1, storage.get());
    manager.setQualityCode(2, 2, storage.get());
    manager.setQualityCode(3, 3, storage.get());
    manager.setQualityCode(4, 4, storage.get());
    manager.setQualityCode(5, 5, storage.get());
    manager.setQualityCode(6, 6, storage.get());
    manager.setQualityCode(7, 7, storage.get());
    manager.setQualityCode(8, 8, storage.get());
    manager.setQualityCode(9, 9, storage.get());

    EXPECT_TRUE(0 == std::memcmp(expected.get(), storage.get(), length));
}

TEST(LowLevelBufferHandler, GetBufferThrowsExceptionWhenInvalidArgumentsIsSent) {
    LowLevelBufferHandler manager(80, 256);
    EXPECT_THROW(manager.getRawBuffer(-1, 0), std::invalid_argument);
    EXPECT_THROW(manager.getRawBuffer(80, 0), std::invalid_argument);
}

TEST(LowLevelBufferHandler, GetQualityThrowsExceptionWhenInvalidArgumentsIsSent) {
    LowLevelBufferHandler manager(80, 256);
    EXPECT_THROW(manager.getQualityCode(-1, 0), std::invalid_argument);
    EXPECT_THROW(manager.getQualityCode(80, 0), std::invalid_argument);
}

TEST(LowLevelBufferHandler, SetQualityThrowsExceptionWhenInvalidArgumentsIsSent) {
    LowLevelBufferHandler manager(80, 256);
    EXPECT_THROW(manager.setQualityCode(-1, 1, 0), std::invalid_argument);
    EXPECT_THROW(manager.setQualityCode(80, 1, 0), std::invalid_argument);
}

//! @note: Пригодится

//for (int i = 0; i < length; ++i)
//    qDebug() << i << (float)(expected.get())[i] << (float)((char*)storage)[i];
//for (int i = 0; i < BUFFERS_COUNT * BUFFER_SIZE; ++i)
//    qDebug() << i << ((SignalValue*)expected)[i] << ((SignalValue*)dump)[i];
