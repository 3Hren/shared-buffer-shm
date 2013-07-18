#include <memory>

#include <QTimer>

#include "domain/MetaData.h"
#include "SharedMemoryLocker.h"
#include "SharedBufferServer.h"
#include "LowLevelBufferHandler.h"

SharedBufferServer::SharedBufferServer(const QString &name, BufferId buffersCount, BufferPos bufferSize, QObject *parent) :
    QObject(parent),
    name(name),
    buffersCount(buffersCount),
    bufferSize(bufferSize),
    handler(new LowLevelBufferHandler(buffersCount, bufferSize)),
    shared(new QSharedMemory(name, this)),
    refreshTimer(new QTimer(this)),
    log(log4cxx::Logger::getLogger("ru.diaprom.sharbuf.Server"))
{
    refreshTimer->setInterval(1000);
    connect(refreshTimer, SIGNAL(timeout()), SLOT(refreshState()));
    LOG4CXX_INFO(log, "Native key: " << shared->nativeKey().toStdString());
}

SharedBufferServer::~SharedBufferServer()
{
    delete handler;
}

void SharedBufferServer::execute()
{
    LowLevelBufferHandler manager(buffersCount, bufferSize);
    bool isCreated = shared->create(manager.getDataLengthBytes());
    LOG4CXX_INFO(log, "Shared memory segment has been created: " << std::boolalpha << isCreated);
    if (!isCreated)
        exit(1);

    SharedMemoryLocker<QSharedMemory> locker(shared);
    void *data = shared->data();
    std::unique_ptr<void> initialized(manager.createStorage());
    memcpy(data, initialized.get(), manager.getDataLengthBytes());
    LOG4CXX_INFO(log, "Shared memory segment has been initialized with " << shared->size() << " bytes (" << shared->size() / 1024.0 / 1024.0 << " Mbytes)");

    refreshTimer->start();
}

void SharedBufferServer::refreshState()
{
    SharedMemoryLocker<QSharedMemory> locker(shared);
    MetaData meta = handler->getMetaData(shared->constData());
    if (meta.clientCount > clientCount) {
        LOG4CXX_INFO(log, "Client has been connected. Total clients: " << meta.clientCount);
    } else if (meta.clientCount < clientCount) {
        LOG4CXX_INFO(log, "Client has been disconnected. Total clients: " << meta.clientCount);
    }

    clientCount = meta.clientCount;
}
