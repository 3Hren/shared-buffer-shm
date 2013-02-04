#include "_SharedBufferStorageClient.h"

#include "LowLevelBufferHandler.h"

#include <QSharedMemory>
#include <QTimer>
#include <QDebug>

#include <log4cxx/logger.h>

_SharedBufferStorageClient::_SharedBufferStorageClient(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent) :
    QObject(parent),
    name(name),
    buffersCount(buffersCount),
    bufferSize(bufferSize),
    timeout(timeout)
{
    shared = new QSharedMemory(name, this);
    manager = new LowLevelBufferHandler(buffersCount, bufferSize);
}

_SharedBufferStorageClient::~_SharedBufferStorageClient()
{
    delete manager;
}

void _SharedBufferStorageClient::connectAndRun()
{
    const bool isAttached = shared->attach();
    LOG4CXX_INFO(log4cxx::Logger::getRootLogger(), "Shared memory segment has been attached: " << std::boolalpha << isAttached);
    if (!shared->isAttached()) {
        LOG4CXX_FATAL(log4cxx::Logger::getRootLogger(), "Shared memory segment attaching failed: " << shared->errorString().toStdString());
        exit(1);
    }
    QTimer::singleShot(timeout, this, SLOT(execute()));
}