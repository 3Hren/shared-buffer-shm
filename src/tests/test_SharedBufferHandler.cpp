#include "Global.h"

TEST(SharedBufferHandler, SharedBufferWriterClass) {
    LowLevelBufferHandler manager(10, 20);
    SharedBufferWriter sharedManager(&manager);
    BufferWriter decorator(&sharedManager);
    Q_UNUSED(decorator);
}
