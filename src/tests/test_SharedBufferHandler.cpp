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

TEST_F(SharedBufferReaderTest, GetBuffer) {
    SignalValue *signalValues = new SignalValue[4]{0.0, 1.0, 2.5, 5.0};
    int length = lowLevelBufferHandler.getDataLengthBytes();
    std::unique_ptr<char[]>data(new char[length]);
    std::memset(data.get(), 0, length);
    std::memcpy(data.get() + sizeof(MetaData) + 1 * sizeof(SignalValue) * lowLevelBufferHandler.getBufferSize(),
                signalValues,
                4 * sizeof(SignalValue));

    EXPECT_CALL(sharedMemory, lock())
            .Times(1);
    EXPECT_CALL(sharedMemory, unlock())
            .Times(1);    
    EXPECT_CALL(sharedMemory, constData())
            .Times(1)
            .WillOnce(Return(data.get()));
    EXPECT_CALL(lowLevelBufferHandler, getQualityCode(1, data.get()))
            .Times(1)
            .WillOnce(Return(512));

    Buffer expected = {
        {0.0, 5.0, 2.5, 1.0},
        512
    };
    EXPECT_EQ(expected, reader.getBuffer(1));
}
