#include "Server.h"

#include "BufferManager.h"

#include <log4cxx/logger.h>

log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("Server");

Server::Server(const QString &name, BufferId buffersCount, BufferSize bufferSize, QObject *parent) :
    QObject(parent),
    name(name),
    buffersCount(buffersCount),
    bufferSize(bufferSize)
{
    shared = new QSharedMemory(name, this);
}

void Server::execute()
{
    BufferManager manager(buffersCount, bufferSize);
    bool isCreated = shared->create(manager.getDataLength());    
    LOG4CXX_INFO(logger, "Shared memory segment has been created: " << std::boolalpha << isCreated);
    if (!isCreated)
        exit(1);

    shared->lock();
    void *data = shared->data();
    void *initialized = manager.createStorage();
    memcpy(data, initialized, manager.getDataLength());
    shared->unlock();
    delete[] (char*)initialized;
    LOG4CXX_INFO(logger, "Shared memory segment has been initialized with " << shared->size() << " bytes (" << shared->size() / 1024.0 / 1024.0 << " Mbytes)");
}
