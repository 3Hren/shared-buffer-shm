#include "_SharedBufferStorageClient.h"

#include "LowLevelBufferHandler.h"
#include "QtBasedSharedMemory.h"

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
    shared = new QtBasedSharedMemory;
    shared->setKey(name);
    manager = new LowLevelBufferHandler(buffersCount, bufferSize);
}

_SharedBufferStorageClient::~_SharedBufferStorageClient()
{    
    delete manager;
    delete shared;
}

void _SharedBufferStorageClient::connectAndRun()
{
    const bool isAttached = shared->attach();
    LOG4CXX_INFO(log4cxx::Logger::getRootLogger(), "Shared memory segment has been attached: " << std::boolalpha << isAttached);
    if (!shared->isAttached()) {
        LOG4CXX_FATAL(log4cxx::Logger::getRootLogger(), "Shared memory segment attaching failed: " << shared->getErrorDescription().toStdString());
        exit(1);
    }
    QTimer::singleShot(timeout, this, SLOT(execute()));
}
