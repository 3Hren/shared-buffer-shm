#include "Global.h"
#include "BufferManager.h"

TEST(BufferManager, InitializationConstructor) {
    BufferManager manager(64, 1024);
    EXPECT_EQ(64, manager.getBuffersCount());
    EXPECT_EQ(1024, manager.getBufferSize());
}

TEST(BufferManager, Push) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferSize BUFFER_SIZE = 4;

    // Подготавливаем пакет с сигналами, который требуется запушить
    SignalValue *signalsPack = new SignalValue[BUFFERS_COUNT];
    for (int i = 0; i < BUFFERS_COUNT; ++i)
        signalsPack[i] = i;

    BufferManager manager(BUFFERS_COUNT, BUFFER_SIZE);    
    void *storage = manager.createStorage();
    manager.push(10, signalsPack, storage);

    SignalValue expectedBuffer[] = {
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
    BufferSize expectedBufferId = 0;
    TimeStamp expectedTimeStamps[] = {10};
    const int length = manager.getDataLength();
    char *expected = new char[length];
    memset(expected, 0, length);

    SignalValue *_expected = (SignalValue*)expected;
    memcpy(_expected, expectedBuffer, (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue));
    memcpy(_expected + BUFFERS_COUNT * BUFFER_SIZE, &expectedBufferId, sizeof(SignalValue));
    memcpy(_expected + BUFFERS_COUNT * BUFFER_SIZE + 1, &expectedTimeStamps, 1 * sizeof(TimeStamp));
    EXPECT_TRUE(0 == std::memcmp(expected, storage, length));

    //! Cleanup
    delete[] expected;
    delete[] (char*)storage;
    delete[] signalsPack;
}

TEST(BufferManager, MultiplePush) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferSize BUFFER_SIZE = 4;

    BufferManager manager(BUFFERS_COUNT, BUFFER_SIZE);    
    void *storage = manager.createStorage();
    for (int j = 0; j < 3; ++j) {
        SignalValue *signalsPack = new SignalValue[BUFFERS_COUNT];
        for (int i = 0; i < BUFFERS_COUNT; ++i)
            signalsPack[i] = i;
        manager.push(j, signalsPack, (void*)storage);
        delete[] signalsPack;
    }

    SignalValue expectedBuffer[] = {
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
    BufferSize expectedBufferId = 2;
    TimeStamp expectedTimeStamps[] = {0, 1, 2};
    const int length = manager.getDataLength();
    char *expected = new char[length];
    memset(expected, 0, length);

    SignalValue *_expected = (SignalValue*)expected;
    memcpy(_expected, expectedBuffer, (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue));
    memcpy(_expected + BUFFERS_COUNT * BUFFER_SIZE, &expectedBufferId, sizeof(SignalValue));
    memcpy(_expected + BUFFERS_COUNT * BUFFER_SIZE + 1, &expectedTimeStamps, 3 * sizeof(TimeStamp));
    EXPECT_TRUE(0 == std::memcmp(expected, storage, length));

    //! Cleanup
    delete[] expected;
    delete[] (char*)storage;
}

TEST(BufferManager, MultiplePushWithFullingBuffer) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferSize BUFFER_SIZE = 4;

    BufferManager manager(BUFFERS_COUNT, BUFFER_SIZE);    
    void *storage = manager.createStorage();
    for (int j = 0; j < 4; ++j) {
        SignalValue *signalsPack = new SignalValue[BUFFERS_COUNT];
        for (int i = 0; i < BUFFERS_COUNT; ++i)
            signalsPack[i] = j;
        manager.push(j, signalsPack, (void*)storage);
        delete[] signalsPack;
    }

    SignalValue expectedBuffer[] = {
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
    BufferSize expectedBufferId = 3;
    TimeStamp expectedTimeStamps[] = {0, 1, 2, 3};
    const int length = manager.getDataLength();
    char *expected = new char[length];
    memset(expected, 0, length);

    SignalValue *_expected = (SignalValue*)expected;
    memcpy(_expected, expectedBuffer, (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue));
    memcpy(_expected + BUFFERS_COUNT * BUFFER_SIZE, &expectedBufferId, sizeof(SignalValue));
    memcpy(_expected + BUFFERS_COUNT * BUFFER_SIZE + 1, &expectedTimeStamps, 4 * sizeof(TimeStamp));
    EXPECT_TRUE(0 == std::memcmp(expected, storage, length));

    //! Cleanup
    delete[] expected;
    delete[] (char*)storage;
}

TEST(BufferManager, MultiplePushWithOverriding) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferSize BUFFER_SIZE = 4;

    BufferManager manager(BUFFERS_COUNT, BUFFER_SIZE);    
    void *storage = manager.createStorage();
    for (int j = 0; j < 5; ++j) {
        SignalValue *signalsPack = new SignalValue[BUFFERS_COUNT];
        for (int i = 0; i < BUFFERS_COUNT; ++i)
            signalsPack[i] = j;
        manager.push(j, signalsPack, (void*)storage);
        delete[] signalsPack;
    }

    SignalValue expectedBuffer[] = {
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
    BufferSize expectedBufferId = 0;
    TimeStamp expectedTimeStamps[] = {4, 1, 2, 3};
    const int length = manager.getDataLength();
    char *expected = new char[length];
    memset(expected, 0, length);

    SignalValue *_expected = (SignalValue*)expected;
    memcpy(_expected, expectedBuffer, (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue));
    memcpy(_expected + BUFFERS_COUNT * BUFFER_SIZE, &expectedBufferId, sizeof(SignalValue));
    memcpy(_expected + BUFFERS_COUNT * BUFFER_SIZE + 1, &expectedTimeStamps, 4 * sizeof(TimeStamp));
    EXPECT_TRUE(0 == std::memcmp(expected, storage, length));

    //! Cleanup
    delete[] expected;
    delete[] (char*)storage;
}

TEST(BufferManager, GetBuffersDump) {
    static const BufferId BUFFERS_COUNT = 10;
    static const BufferSize BUFFER_SIZE = 4;

    BufferManager manager(BUFFERS_COUNT, BUFFER_SIZE);
    const int length = manager.getDataLength();
    char *storage = new char[length];
    memset(storage, 0, length);
    SignalValue bufferDump[] = {
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
    BufferSize bufferId = 0;
    TimeStamp timeStamps[] = {4, 1, 2, 3};

    SignalValue *_storage = (SignalValue*)storage;
    memcpy(_storage, bufferDump, (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue));
    memcpy(_storage + BUFFERS_COUNT * BUFFER_SIZE, &bufferId, sizeof(SignalValue));
    memcpy(_storage + BUFFERS_COUNT * BUFFER_SIZE + 1, &timeStamps, 4 * sizeof(TimeStamp));    

    char *expected = new char[manager.getDumpLength()];
    memset(expected, 0, manager.getDumpLength());
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
    TimeStamp expectedTimeStamps[] = {4, 3, 2, 1};
    SignalValue *_expected = (SignalValue*)expected;
    memcpy(_expected, expectedBufferDump, (BUFFERS_COUNT * BUFFER_SIZE) * sizeof(SignalValue));
    memcpy(_expected + BUFFERS_COUNT * BUFFER_SIZE, &expectedTimeStamps, 4 * sizeof(TimeStamp));

    void *dump = manager.getBuffersDump((void*)storage);
    EXPECT_TRUE(0 == std::memcmp(expected, dump, length - 4));

    //! Cleanup
    delete[] storage;
    delete[] expected;
    delete[] (char*)dump;
}

//! @note: Пригодится
//for (int i = 0; i < length; ++i)
    //qDebug() << i << (float)((float*)expected)[i] << (float)((float*)byteArray)[i];
    //qDebug() << i << (float)expected[i] << (float)byteArray[i];
//void *r = manager.getBuffersDump((void*)storage);
//for (int i = 0; i < length; ++i)
//    qDebug() << i << (float)(expected)[i] << (float)((char*)r)[i];
//    //qDebug() << i << (float)_expected[i] << (float)((float*)r)[i];
