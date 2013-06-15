#include "_Dumper.h"

#include "LowLevelBufferHandler.h"
#include "SharedBufferReader.h"

#include <log4cxx/logger.h>
#include <boost/timer/timer.hpp>

#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>

#include <QDebug>

_Dumper::_Dumper(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent) :
    _SharedBufferStorageClient(name, buffersCount, bufferSize, timeout, parent),
    reader(new SharedBufferReader)
{    
    reader->setLowLevelBufferHandler(manager);
    reader->setSharedMemory(shared);
}

_Dumper::~_Dumper()
{
    delete reader;
}

void _Dumper::execute()
{
    //readBuffer();
    dump();
}

void _Dumper::dump()
{
    QTimer::singleShot(timeout, this, SLOT(execute()));
    static int counter = 0;
    counter++;

    boost::timer::cpu_timer timer;
    BuffersDump dump = reader->getBuffersDump();
    LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), "#" << counter << ". Dumped " << buffersCount * bufferSize << " values in " << timer.elapsed().wall / 1.0e6 << " ms");
#ifdef DEBUG
    for (const Buffer &buffer : dump.buffers) {
        qDebug() << buffer;
    }
#endif
}

void _Dumper::readBuffer()
{
    QTimer::singleShot(timeout, this, SLOT(execute()));
    static int counter = 0;
    counter++;

    boost::timer::cpu_timer timer;
    const Buffer &buffer = reader->getBuffer<QVector>(0);
    LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), "#" << counter << ". Buffer with size " << buffer.values.size() << " read in " << timer.elapsed().wall / 1.0e6 << " ms");
}
