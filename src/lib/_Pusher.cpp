#include "_Pusher.h"

#include "LowLevelBufferManager.h"

#include <log4cxx/logger.h>

#include <QSharedMemory>
#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>
#include <QElapsedTimer>

_Pusher::_Pusher(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent) :
    SharedBufferStorageClient(name, buffersCount, bufferSize, timeout, parent)
{
    s = new SharedBufferWriter(manager);
    s->attach(name);
    m = new BufferManager(s);
    m->start();
}

void _Pusher::execute()
{
    push();
}

void _Pusher::push()
{
    QTimer::singleShot(timeout, this, SLOT(push()));
    static int counter = 0;
    counter++;
    SignalValue *data = new SignalValue[buffersCount];
    for (int i = 0; i < buffersCount; ++i)
        data[i] = counter;

    QElapsedTimer timer;
    timer.start();
    m->push(QDateTime::currentDateTime().toMSecsSinceEpoch(), data);
    delete[] data;

    LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), "#" << counter << ". Pushed to queue " << buffersCount << " values in " << timer.elapsed() << " ms");
}
