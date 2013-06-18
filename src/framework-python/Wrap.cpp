#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/numeric.hpp>

#include "Python.h"
#include "arrayobject.h"

#include "QtBasedSharedMemory.h"
#include "LowLevelBufferHandler.h"
#include "SharedBufferReader.h"
#include "SharedMemoryLocker.h"

#include <QDebug>

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
    }

    bool isAttached() const {
        return reader.isAttached();
    }

    void attach(const std::string &key) {
        reader.attach(key.c_str());
    }

    void getBuffer(BufferId bufferId, boost::python::numeric::array &array) const {
        getBuffer(bufferId, handler.getBufferSize(), array);
    }

    void getBuffer(BufferId bufferId, BufferId size, boost::python::numeric::array &array) const {
        PyArrayObject *arr = (PyArrayObject*)PyArray_FROM_O(array.ptr());
        float *data = reinterpret_cast<float*>(arr->data);
        TypedBuffer<std::vector> buf = reader.getBuffer<std::vector>(bufferId, size);
        std::copy(buf.values.begin(), buf.values.end(), data);
    }
};

using namespace boost::python;

BOOST_PYTHON_MODULE(libpysharbuf) {
    import_array();
    boost::python::numeric::array::set_module_and_type("numpy", "ndarray");

    class_<std::vector<SignalValue>>("SignalValues")
           .def(vector_indexing_suite<std::vector<SignalValue>, true>());

    class_<TypedBuffer<std::vector>>("Buffer", init<>())
            .add_property("values", make_getter(&TypedBuffer<std::vector>::values), make_setter(&TypedBuffer<std::vector>::values))
            .add_property("quality", make_getter(&TypedBuffer<std::vector>::quality), make_setter(&TypedBuffer<std::vector>::quality));

    class_<BufferReader>("BufferReader", init<BufferId, BufferPos>(args("buffersCount", "bufferSize")))            
            .def("isAttached", &BufferReader::isAttached)
            .def("attach", &BufferReader::attach, args("key"))
            .def("getBuffer", static_cast<void (BufferReader::*)(BufferId, boost::python::numeric::array&) const>(&BufferReader::getBuffer), args("bufferId", "array"))
            .def("getBuffer", static_cast<void (BufferReader::*)(BufferId, BufferId, boost::python::numeric::array&) const>(&BufferReader::getBuffer), args("bufferId", "size", "array"));
}
