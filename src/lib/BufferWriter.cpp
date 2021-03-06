#include "config.hpp"

#include <thread>

#ifdef SHARBUF_DEBUG
#include <QDateTime>
#endif

#include "domain/SignalPack.h"
#include "exceptions/SharedBufferException.h"
#include "BufferWriter.h"
#include "SharedBufferWriter.h"

BufferWriter::BufferWriter(SharedBufferWriter *sharedBufferWriter) :
    done(true),
    sharedBufferWriter(sharedBufferWriter),
    log(log4cxx::Logger::getLogger("ru.diaprom.sharbuf.BufferWriter"))
{
}

BufferWriter::~BufferWriter()
{
    done = true;
    consumer.join();
}

bool BufferWriter::isRunning() const {
    return !done;
}

void BufferWriter::start()
{
    if (isRunning())
        throw IllegalStateException("Writer is already running");
    done = false;

    std::thread consumer([this]{
        std::chrono::milliseconds timeout(100);
        while (!done) {
            SignalPack signalPack;
            if (queue.tryPop(signalPack, timeout)) {
                sharedBufferWriter->push(signalPack.timeStamp, signalPack.signalValues.data());
#ifdef SHARBUF_DEBUG
                LOG4CXX_DEBUG(log, "Pushed signal pack with timestamp: " << QDateTime::fromMSecsSinceEpoch(signalPack.timeStamp).toString("ss,zzz").toStdString());
#endif
            }
        }
        LOG4CXX_DEBUG(log, "Consumer thread has been stopped");
    });
    this->consumer = std::move(consumer);
}

void BufferWriter::push(TimeStamp timeStamp, SignalValue *signalValues)
{
    SignalPack pack(timeStamp, signalValues, sharedBufferWriter->getBuffersCount());
    queue.push(std::move(pack));
}

void BufferWriter::push(const SignalPack &pack)
{
    queue.push(pack);
}
