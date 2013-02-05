#include "Mocks.h"

TEST(SharedBufferWriter, Class) {
    SharedBufferWriter writer;
    Q_UNUSED(writer);
}

class SharedBufferWriterTest : public Test {
protected:
    NiceMock<SharedMemoryMock> sharedMemory;
    NiceMock<LowLevelBufferHandlerMock> lowLevelBufferHandler;
    SharedBufferWriter writer;
    void SetUp() {
        writer.setLowLevelBufferHandler(&lowLevelBufferHandler);
        writer.setSharedMemory(&sharedMemory);

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

/*! @todo:
 * Привет, я из будущего!
 * Пишу тебе этот абзац, чтобы вспомнить то, что писал.
 * Закончил я на этом тесте. Все теги todo описаны.
 * Продолжать разработку классов SharedBufferReader/Writer.
 * Внедрять новые низкоуровневые методы.
 * Разобраться с высокоуровневыми методами.
 */

TEST(SharedBufferReader, Class) {
    SharedBufferReader reader;
    Q_UNUSED(reader);
}
