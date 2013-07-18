#include <chrono>

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
    std::unique_ptr<SignalValue[]>data(new SignalValue[buffersCount]);
    for (BufferId i = 0; i < buffersCount; ++i)
        data[i] = counter;

    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    writer->push(QDateTime::currentDateTime().toMSecsSinceEpoch(), data.get());
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    LOG4CXX_DEBUG(log, "#" << counter << ". Pushed to queue " << buffersCount << " values in " << time_span.count() * 1000 << " ms");
}
