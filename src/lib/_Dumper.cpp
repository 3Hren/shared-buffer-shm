#include "_Dumper.h"

#include "LowLevelBufferHandler.h"

#include <log4cxx/logger.h>

#include <QSharedMemory>
#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>
#include <QElapsedTimer>

_Dumper::_Dumper(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent) :
    _SharedBufferStorageClient(name, buffersCount, bufferSize, timeout, parent)
{
}

void _Dumper::execute()
{
    dump();
}

void _Dumper::dump()
{
    QTimer::singleShot(timeout, this, SLOT(dump()));
    static int counter = 0;
    counter++;

    QElapsedTimer timer;
    timer.start();    
    shared->lock();
    LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), "#" << counter << ". Lock acquired");
    char *data = manager->getBuffersDump(shared->data());
    LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), "#" << counter << ". Dumped " << buffersCount * bufferSize << " values in " << timer.elapsed() << " ms");
    shared->unlock();    

    delete[] data;    
}
