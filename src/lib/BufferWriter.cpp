#include "BufferWriter.h"

#include "SharedBufferWriter.h"

#include <thread>
#include <log4cxx/logger.h>

#include <QDateTime>
#include <QDebug>

BufferWriter::BufferWriter(SharedBufferWriter *sharedBufferWriter) :
    done(false),
    sharedBufferWriter(sharedBufferWriter)
{
}

BufferWriter::~BufferWriter()
{
    done = true;
}

void BufferWriter::start()
{
    std::thread consumer([this]{
        while (!done) {
            std::shared_ptr<SignalPack> signalPack = queue.blockingPop();
            sharedBufferWriter->push(signalPack->timeStamp, signalPack->signalValues.data());
            LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), "Pushed signal pack with timestamp: " << QDateTime::fromMSecsSinceEpoch(signalPack->timeStamp).toString("ss,zzz").toStdString());
        }
    });
    this->consumer = std::move(consumer);
}

void BufferWriter::push(TimeStamp timeStamp, SignalValue *signalValues)
{
    SignalPack pack(timeStamp, signalValues, sharedBufferWriter->getBuffersCount());
    queue.push(pack);
}
