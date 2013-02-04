#include "Global.h"

TEST(BufferWriter, Class) {
    LowLevelBufferHandler manager(10, 20);
    SharedBufferWriter sharedManager(&manager);
    BufferWriter decorator(&sharedManager);
    Q_UNUSED(decorator);
}

TEST(SharedBufferReader, Class) {
    LowLevelBufferHandler lowLevelBufferHandler(10, 20);
    SharedBufferReader reader(&lowLevelBufferHandler);
    Q_UNUSED(reader);
}
