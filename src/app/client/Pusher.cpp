#include <boost/timer/timer.hpp>
#include <boost/scoped_array.hpp>

#include <QTimer>
#include <QDateTime>

#include "exceptions/SharedBufferException.h"
#include "QtBasedSharedMemory.h"
#include "LowLevelBufferHandler.h"
#include "Pusher.hpp"

Pusher::Pusher(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent) :
    SharedBufferStorageClient(name, buffersCount, bufferSize, timeout, parent),
    log(log4cxx::Logger::getLogger("ru.diaprom.sharbuf.Pusher"))
{
    sharedBufferWriter = new SharedBufferWriter;
    sharedBufferWriter->setSharedMemory(shared);
    sharedBufferWriter->setLowLevelBufferHandler(manager);

    writer = new BufferWriter(sharedBufferWriter);

}

Pusher::~Pusher()
{
    delete writer;
    delete sharedBufferWriter;
}

void Pusher::execute()
{
    if (!writer->isRunning())
        writer->start();
    push();
}

void Pusher::push()
{
    QTimer::singleShot(timeout, this, SLOT(execute()));

    static int counter = 0;
    counter++;
    boost::scoped_array<SignalValue>data(new SignalValue[buffersCount]);
    for (BufferId i = 0; i < buffersCount; ++i)
        data[i] = counter;

    boost::timer::cpu_timer timer;
    writer->push(QDateTime::currentDateTime().toMSecsSinceEpoch(), data.get());
    LOG4CXX_DEBUG(log, "#" << counter << ". Pushed to queue " << buffersCount << " values in " << timer.elapsed().wall / 1.0e6 << " ms");
}
