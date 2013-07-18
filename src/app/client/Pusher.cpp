#include <boost/timer/timer.hpp>
#include <boost/scoped_array.hpp>

#include <QTimer>
#include <QDateTime>

#include "exceptions/SharedBufferException.h"
#include "QtBasedSharedMemory.h"
#include "LowLevelBufferHandler.h"
#include "Pusher.hpp"

Pusher::Pusher(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent) :
    QObject(parent),
    buffersCount(buffersCount),
    bufferSize(bufferSize),
    timeout(timeout),
    log(log4cxx::Logger::getLogger("ru.diaprom.sharbuf.Pusher"))
{
    lowLevelBufferHandler = new LowLevelBufferHandler(buffersCount, bufferSize);
    sharedMemory = new QtBasedSharedMemory;

    sharedBufferWriter = new SharedBufferWriter;
    sharedBufferWriter->setSharedMemory(sharedMemory);
    sharedBufferWriter->setLowLevelBufferHandler(lowLevelBufferHandler);

    try {
        sharedBufferWriter->attach(name);
        LOG4CXX_INFO(log, "Shared memory segment has been successfully attached");
    } catch (AttachException &e) {
        LOG4CXX_FATAL(log, "Shared memory segment attaching failed: " << sharedMemory->getErrorDescription().toStdString());
        exit(1);
    }

    writer = new BufferWriter(sharedBufferWriter);
    writer->start();
}

Pusher::~Pusher()
{
    delete writer;
    delete sharedBufferWriter;
    delete sharedMemory;
    delete lowLevelBufferHandler;
}

void Pusher::execute()
{
    push();
}

void Pusher::push()
{
    static int counter = 0;
    counter++;
    boost::scoped_array<SignalValue>data(new SignalValue[buffersCount]);
    for (BufferId i = 0; i < buffersCount; ++i)
        data[i] = counter;

    boost::timer::cpu_timer timer;
    writer->push(QDateTime::currentDateTime().toMSecsSinceEpoch(), data.get());
    LOG4CXX_DEBUG(log, "#" << counter << ". Pushed to queue " << buffersCount << " values in " << timer.elapsed().wall / 1.0e6 << " ms");

    QTimer::singleShot(timeout, this, SLOT(push()));
}
