#include "SharedBufferStorageClient.h"

#include "LowLevelBufferManager.h"

#include <QSharedMemory>
#include <QTimer>
#include <QDebug>

#include <log4cxx/logger.h>

SharedBufferStorageClient::SharedBufferStorageClient(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent) :
    QObject(parent),
    name(name),
    buffersCount(buffersCount),
    bufferSize(bufferSize),
    timeout(timeout)
{
    shared = new QSharedMemory(name, this);
    manager = new LowLevelBufferManager(buffersCount, bufferSize);
}

SharedBufferStorageClient::~SharedBufferStorageClient()
{
    delete manager;
}

void SharedBufferStorageClient::connectAndRun()
{
    const bool isAttached = shared->attach();
    LOG4CXX_INFO(log4cxx::Logger::getRootLogger(), "Shared memory segment has been attached: " << std::boolalpha << isAttached);
    if (!shared->isAttached()) {
        LOG4CXX_FATAL(log4cxx::Logger::getRootLogger(), "Shared memory segment attaching failed: " << shared->errorString().toStdString());
        exit(1);
    }
    QTimer::singleShot(timeout, this, SLOT(execute()));
}

void SharedBufferStorageClient::showBufferDump(void *data)
{
    for (BufferId id = 0; id < buffersCount; ++id) {
        QString line;
        for (BufferPos pos = 0; pos < bufferSize; ++pos)
            line += QString::number(((float*)data)[1 + id * bufferSize + pos], 'f', 2).leftJustified(10, ' ');
        qDebug() << id << " " << line;
    }
}
