#include "Server.h"

#include "BufferManager.h"

#include <QDebug>

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
    shared->lock();
    void *data = shared->data();
    void *initialized = manager.createStorage();
    memcpy(data, initialized, manager.getDataLength());
    shared->unlock();
    delete[] (char*)initialized;
    qDebug() << "Shared memory segment created:" << isCreated;
}
