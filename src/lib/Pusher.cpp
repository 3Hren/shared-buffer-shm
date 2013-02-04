#include "Pusher.h"

#include "BufferManager.h"

#include <log4cxx/logger.h>

#include <QSharedMemory>
#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>
#include <QElapsedTimer>

#include <QDebug>

Pusher::Pusher(const QString &name, BufferId buffersCount, BufferSize bufferSize, int timeout, QObject *parent) :
    SharedBufferStorageClient(name, buffersCount, bufferSize, timeout, parent)
{
}

void Pusher::execute()
{
    push();
}

void Pusher::push()
{
    QTimer::singleShot(timeout, this, SLOT(push()));
    static int counter = 0;
    counter++;
    SignalValue *data = new SignalValue[buffersCount];
    for (int i = 0; i < buffersCount; ++i)
        data[i] = counter;

    QElapsedTimer timer;
    timer.start();
    shared->lock();
    manager->push(QDateTime::currentDateTime().toMSecsSinceEpoch(), data, shared->data());
    shared->unlock();
    delete[] data;

    LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), "#" << counter << ". Pushed " << buffersCount << " values in " << timer.elapsed() << " ms");
}
