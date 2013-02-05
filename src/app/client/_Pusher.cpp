#include "_Pusher.h"

#include "LowLevelBufferHandler.h"

#include "QtBasedSharedMemory.h"

#include <log4cxx/logger.h>

#include <boost/timer/timer.hpp>
#include <boost/scoped_array.hpp>

#include <QTimer>
#include <QDateTime>

_Pusher::_Pusher(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent) :
    QObject(parent),
    buffersCount(buffersCount),
    bufferSize(bufferSize),
    timeout(timeout)
{
    lowLevelBufferHandler = new LowLevelBufferHandler(buffersCount, bufferSize);
    sharedMemory = new QtBasedSharedMemory;

    sharedBufferWriter = new SharedBufferWriter;
    sharedBufferWriter->setSharedMemory(sharedMemory);
    sharedBufferWriter->setLowLevelBufferHandler(lowLevelBufferHandler);
    sharedBufferWriter->attach(name);

    writer = new BufferWriter(sharedBufferWriter);
    writer->start();
}

_Pusher::~_Pusher()
{
    delete sharedMemory;
}

void _Pusher::execute()
{
    push();
}

void _Pusher::push()
{    
    static int counter = 0;
    counter++;
    boost::scoped_array<SignalValue>data(new SignalValue[buffersCount]);
    for (int i = 0; i < buffersCount; ++i)
        data[i] = counter;

    boost::timer::cpu_timer timer;
    writer->push(QDateTime::currentDateTime().toMSecsSinceEpoch(), data.get());
    LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), "#" << counter << ". Pushed to queue " << buffersCount << " values in " << timer.elapsed().wall / 1.0e6 << " ms");

    QTimer::singleShot(timeout, this, SLOT(push()));
}