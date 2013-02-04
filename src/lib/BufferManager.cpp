#include "BufferManager.h"

#include "SharedBufferWriter.h"

#include <boost/thread.hpp>

#include <log4cxx/logger.h>

#include <QDateTime>
#include <QDebug>

BufferManager::BufferManager(SharedBufferWriter *manager) :
    manager(manager)
{
}

void BufferManager::start()
{
    boost::thread consumer([&](){
        boost::mutex::scoped_lock lock(packsMutex);
        while (true) {
            while (signalPacks.empty())
                packsAvailableCondition.wait(lock);

            const SignalPack &signalPack = signalPacks.front();
            signalPacks.pop();
            packsMutex.unlock();
            manager->push(signalPack.timeStamp, signalPack.signalValues.data());
            LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), "Pushed signal pack with timestamp: " << QDateTime::fromMSecsSinceEpoch(signalPack.timeStamp).toString("ss:zzz").toStdString());
        }
    });
    Q_UNUSED(consumer);
}

void BufferManager::push(TimeStamp timeStamp, SignalValue *signalValues)
{
    SignalPack pack(timeStamp, signalValues, manager->getBuffersCount());
    boost::mutex::scoped_lock lock(packsMutex);    
    signalPacks.push(pack);
    packsAvailableCondition.notify_one();
    Q_UNUSED(lock);
}
