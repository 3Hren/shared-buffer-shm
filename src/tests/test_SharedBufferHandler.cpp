#include "Mocks.h"

TEST(SharedBufferWriter, Class) {
    SharedBufferWriter writer(0, 0);
    Q_UNUSED(writer);
}

TEST(SharedBufferWriter, ThrowsExceptionWhenTryingToPushAndSharedMemoryIsNotAttached) {
    SharedBufferWriter writer(10, 10);
    SharedMemoryMock sharedMemory;
    EXPECT_CALL(sharedMemory, isAttached())
            .Times(1)
            .WillOnce(Return(false));
    EXPECT_CALL(sharedMemory, getErrorDescription())
            .Times(1)
            .WillOnce(Return(""));
    writer.setSharedMemory(&sharedMemory);

    EXPECT_THROW(writer.push(0, 0), SharedBufferNotAttachedException);
}

TEST(SharedBufferReader, Class) {
    SharedBufferReader reader(0, 0);
    Q_UNUSED(reader);
}
