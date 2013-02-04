#include "Dumper.h"

#include "BufferManager.h"

#include <log4cxx/logger.h>

#include <QSharedMemory>
#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>
#include <QElapsedTimer>

#include <QDebug>

Dumper::Dumper(const QString &name, BufferId buffersCount, BufferSize bufferSize, int timeout, QObject *parent) :
    SharedBufferStorageClient(name, buffersCount, bufferSize, timeout, parent)
{
}

void Dumper::execute()
{
    dump();
}

void Dumper::dump()
{
    QTimer::singleShot(timeout, this, SLOT(dump()));
    static int counter = 0;
    counter++;

    QElapsedTimer timer;
    timer.start();
    shared->lock();
    void *data = manager->getBuffersDump(shared->data());
    shared->unlock();

    LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), "#" << counter << ". Dumped " << buffersCount * bufferSize << " values in " << timer.elapsed() << " ms");
    delete[] (char*)data;    
}
