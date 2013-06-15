#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "QtBasedSharedMemory.h"
#include "LowLevelBufferHandler.h"
#include "SharedBufferReader.h"

#include <QDebug>

typedef std::vector<SignalValue> SignalValues;

class BufferReader {
    QtBasedSharedMemory memory;
    LowLevelBufferHandler handler;
    SharedBufferReader reader;
public:
    BufferReader(BufferId buffersCount, BufferPos bufferSize) :
        handler(buffersCount, bufferSize)
    {
        reader.setSharedMemory(&memory);
        reader.setLowLevelBufferHandler(&handler);
        reader.attach("diaprom");
    }

    TypedBuffer<std::vector> getBuffer(BufferId bufferId) const {
        qDebug() << bufferId;
        return reader.getBuffer<std::vector>(bufferId);
    }   
};

using namespace boost::python;

BOOST_PYTHON_MODULE(libpysharbuf) {
    class_<SignalValues>("SignalValues")
           .def(vector_indexing_suite<SignalValues>());

    class_<TypedBuffer<std::vector>>("Buffer", init<>())
            .add_property("values", make_getter(&TypedBuffer<std::vector>::values), make_setter(&TypedBuffer<std::vector>::values))
            .add_property("quality", make_getter(&TypedBuffer<std::vector>::quality), make_setter(&TypedBuffer<std::vector>::quality));

    class_<BufferReader>("BufferReader", init<BufferId, BufferPos>(args("buffersCount", "bufferSize")))
            .def("getBuffer", &BufferReader::getBuffer, args("bufferId"));
}
