#include "Mocks.h"

TEST(SharedBufferWriter, Class) {
    SharedBufferWriter writer(0, 0);
    Q_UNUSED(writer);
}

TEST(SharedBufferReader, Class) {
    SharedBufferReader reader(0, 0);
    Q_UNUSED(reader);
}
