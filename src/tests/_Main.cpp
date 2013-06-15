#include "Global.h"

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    BeanFactory factory;
    BeanFactory::registerFactory(&factory);
    return RUN_ALL_TESTS();
}
