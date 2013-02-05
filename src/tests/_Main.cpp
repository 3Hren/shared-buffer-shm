#include "Global.h"

#include <boost/scoped_ptr.hpp>

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    boost::scoped_ptr<BeanFactory>factory(new BeanFactory());
    BeanFactory::registerFactory(factory.get());
    return RUN_ALL_TESTS();
}
