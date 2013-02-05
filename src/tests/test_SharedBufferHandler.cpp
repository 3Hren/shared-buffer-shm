#include "Mocks.h"

TEST(SharedBufferWriter, Class) {
    LowLevelBufferHandlerMock lowLevelBufferHandler;
    SharedBufferWriter writer(&lowLevelBufferHandler);
    Q_UNUSED(writer);
}

TEST(SharedBufferReader, Class) {
    LowLevelBufferHandlerMock lowLevelBufferHandler;
    SharedBufferReader reader(&lowLevelBufferHandler);
    Q_UNUSED(reader);
}
