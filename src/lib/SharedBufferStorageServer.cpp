#include "SharedBufferStorageServer.h"

#include "LowLevelBufferHandler.h"

#include <log4cxx/logger.h>

SharedBufferStorageServer::SharedBufferStorageServer(const QString &name, BufferId buffersCount, BufferPos bufferSize, QObject *parent) :
    QObject(parent),
    name(name),
    buffersCount(buffersCount),
    bufferSize(bufferSize)
{
    shared = new QSharedMemory(name, this);
}

void SharedBufferStorageServer::execute()
{
    LowLevelBufferHandler manager(buffersCount, bufferSize);
    bool isCreated = shared->create(manager.getDataLength());    
    LOG4CXX_INFO(log4cxx::Logger::getRootLogger(), "Shared memory segment has been created: " << std::boolalpha << isCreated);
    if (!isCreated)
        exit(1);

    shared->lock();
    void *data = shared->data();
    void *initialized = manager.createStorage();
    memcpy(data, initialized, manager.getDataLength());
    shared->unlock();
    delete[] (char*)initialized;
    LOG4CXX_INFO(log4cxx::Logger::getRootLogger(), "Shared memory segment has been initialized with " << shared->size() << " bytes (" << shared->size() / 1024.0 / 1024.0 << " Mbytes)");
}
