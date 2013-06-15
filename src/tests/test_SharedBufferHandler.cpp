#include "Mocks.h"

TEST(SharedBufferWriter, Class) {
    SharedBufferWriter writer;
    Q_UNUSED(writer);
}

class SharedBufferHandlerTest : public Test {
protected:
    NiceMock<SharedMemoryMock> sharedMemory;
    NiceMock<LowLevelBufferHandlerMock> lowLevelBufferHandler;
    void SetUp() {
        ON_CALL(sharedMemory, isAttached())
                .WillByDefault(Return(true));
        ON_CALL(sharedMemory, getErrorDescription())
                .WillByDefault(Return(""));
        ON_CALL(sharedMemory, lock())
                .WillByDefault(Return(true));
        ON_CALL(sharedMemory, unlock())
                .WillByDefault(Return(true));
    }
};

class SharedBufferWriterTest : public SharedBufferHandlerTest {
protected:
    SharedBufferWriter writer;
    void SetUp() {
        SharedBufferHandlerTest::SetUp();
        writer.setLowLevelBufferHandler(&lowLevelBufferHandler);
        writer.setSharedMemory(&sharedMemory);
    }
};

TEST_F(SharedBufferWriterTest, ThrowsExceptionWhenTryingToPushAndSharedMemoryIsNotAttached) {
    EXPECT_CALL(sharedMemory, isAttached())
            .Times(1)
            .WillOnce(Return(false));

    EXPECT_THROW(writer.push(0, 0), SharedBufferNotAttachedException);
}

TEST_F(SharedBufferWriterTest, CallsLowLevelBufferHandlerPushMethodWhenPushInvoked) {
    TimeStamp timestamp = 10;
    SignalValue signalsPack[] = {1, 2, 3, 4, 5};

    EXPECT_CALL(lowLevelBufferHandler, push(timestamp, signalsPack, _))
            .Times(1);
    EXPECT_CALL(sharedMemory, lock())
            .Times(1);
    EXPECT_CALL(sharedMemory, data())
            .Times(1)
            .WillOnce(Return(nullptr));
    EXPECT_CALL(sharedMemory, unlock())
            .Times(1);

    writer.push(timestamp, signalsPack);
}

TEST(SharedBufferReader, Class) {
    SharedBufferReader reader;
    Q_UNUSED(reader);
}

class SharedBufferReaderTest : public SharedBufferHandlerTest {
protected:
    SharedBufferReader reader;
    void SetUp() {
        SharedBufferHandlerTest::SetUp();
        reader.setLowLevelBufferHandler(&lowLevelBufferHandler);
        reader.setSharedMemory(&sharedMemory);
    }
};

//! @todo: До лучших времен закомментирую. Надо либо пересмотреть тест, либо полностью переходить на статический полиморфизм
//TEST_F(SharedBufferReaderTest, GetBuffer) {
//    EXPECT_CALL(sharedMemory, lock())
//            .Times(1);
//    EXPECT_CALL(sharedMemory, unlock())
//            .Times(1);
//    EXPECT_CALL(sharedMemory, constData())
//            .Times(1)
//            .WillOnce(Return(nullptr));
//    EXPECT_CALL(lowLevelBufferHandler, getBuffer(1, _))
//            .Times(1)
//            .WillOnce(Return(QVector<SignalValue>({0.0, 5.0, 2.5, 1.0})));
//    EXPECT_CALL(lowLevelBufferHandler, getQualityCode(1, _))
//            .Times(1)
//            .WillOnce(Return(512));

//    Buffer expected = {
//        {0.0, 5.0, 2.5, 1.0},
//        512
//    };
//    EXPECT_EQ(expected, reader.getBuffer<QVector>(1));
//}

TEST_F(SharedBufferReaderTest, GetTimestamps) {
    EXPECT_CALL(sharedMemory, lock())
            .Times(1);
    EXPECT_CALL(sharedMemory, unlock())
            .Times(1);
    EXPECT_CALL(sharedMemory, constData())
            .Times(1)
            .WillOnce(Return(nullptr));
    EXPECT_CALL(lowLevelBufferHandler, getTimestamps(_))
            .Times(1)
            .WillOnce(Return(QVector<TimeStamp>({8, 7, 6, 5})));
    reader.getTimestamps();
}

TEST_F(SharedBufferReaderTest, GetBuffersDump) {
    std::unique_ptr<char[]> data(new char[lowLevelBufferHandler.getDataLengthBytes()]);
    EXPECT_CALL(sharedMemory, lock())
            .Times(1);
    EXPECT_CALL(sharedMemory, unlock())
            .Times(1);
    EXPECT_CALL(sharedMemory, constData())
            .Times(1)
            .WillOnce(Return(data.get()));

    const BufferId count = lowLevelBufferHandler.getBuffersCount();
    EXPECT_CALL(lowLevelBufferHandler, getTimestamps(_))
            .Times(1)
            .WillOnce(Return(QVector<TimeStamp>()));
    EXPECT_CALL(lowLevelBufferHandler, getBuffer(AnyOf(Ge(0), Le(count - 1)), _))
            .Times(count)
            .WillRepeatedly(Return(QVector<SignalValue>()));
    EXPECT_CALL(lowLevelBufferHandler, getQualityCode(AnyOf(Ge(0), Le(count - 1)), _))
            .Times(count)
            .WillRepeatedly(Return(0));

    reader.getBuffersDump();
}

TEST_F(SharedBufferReaderTest, GetQualityCode) {
    EXPECT_CALL(sharedMemory, lock())
            .Times(1);
    EXPECT_CALL(sharedMemory, unlock())
            .Times(1);
    EXPECT_CALL(sharedMemory, constData())
            .Times(1)
            .WillOnce(Return(nullptr));
    EXPECT_CALL(lowLevelBufferHandler, getQualityCode(0, _))
            .Times(1)
            .WillOnce(Return(512));
    reader.getQualityCode(0);
}
