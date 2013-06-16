#include "BufferWriter.h"

#include "config.hpp"

#include <thread>

#include "SharedBufferWriter.h"

#ifdef SHARBUF_DEBUG
#include <QDateTime>
#endif

BufferWriter::BufferWriter(SharedBufferWriter *sharedBufferWriter) :
    done(false),
    sharedBufferWriter(sharedBufferWriter),
    log(log4cxx::Logger::getLogger("ru.diaprom.sharbuf.BufferWriter"))
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
#ifdef SHARBUF_DEBUG
            LOG4CXX_DEBUG(log, "Pushed signal pack with timestamp: " << QDateTime::fromMSecsSinceEpoch(signalPack->timeStamp).toString("ss,zzz").toStdString());
#endif
        }        
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
