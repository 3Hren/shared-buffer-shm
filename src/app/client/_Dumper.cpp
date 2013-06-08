#include "_Dumper.h"

#include "LowLevelBufferHandler.h"
#include "SharedBufferReader.h"

#include <log4cxx/logger.h>

#include <QSharedMemory>
#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>
#include <QElapsedTimer>
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
    readBuffer();
    //dump();
}

void _Dumper::dump()
{
    QTimer::singleShot(timeout, this, SLOT(execute()));
    static int counter = 0;
    counter++;

    QElapsedTimer timer;
    timer.start();    
    shared->lock();
    LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), "#" << counter << ". Lock acquired");
    char *data = manager->getBuffersDump(shared->data());
    LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), "#" << counter << ". Dumped " << buffersCount * bufferSize << " values in " << timer.elapsed() << " ms");
    shared->unlock();    
//    QVector<SignalValue> v;
//    for (int i = 0; i < bufferSize; ++i) {
//        SignalValue f;
//        memcpy(&f, data + sizeof(SignalValue) * i, sizeof(SignalValue));
//        v.append(f);
//    }
//    qDebug() << v;

    delete[] data;
}

void _Dumper::readBuffer()
{
    QTimer::singleShot(timeout, this, SLOT(execute()));
    static int counter = 0;
    counter++;

    QElapsedTimer timer;
    timer.start();
    const Buffer &buffer = reader->getBuffer(0);
    LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), "#" << counter << ". Buffer with size " << buffer.values.size() << " read in " << timer.elapsed() << " ms");
}
