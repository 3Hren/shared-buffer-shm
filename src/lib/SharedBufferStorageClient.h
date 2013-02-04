#pragma once

#include <QObject>
#include "SharedBufferGlobal.h"

class QSharedMemory;
class BufferManager;
class SharedBufferStorageClient : public QObject
{
    Q_OBJECT
protected:
    QString name;
    BufferId buffersCount;
    BufferSize bufferSize;
    int timeout;
    BufferManager *manager;
    QSharedMemory *shared;
public:
    SharedBufferStorageClient(const QString &name, BufferId buffersCount, BufferSize bufferSize, int timeout, QObject *parent = 0);
    ~SharedBufferStorageClient();

    void connectAndRun();

protected:
    Q_SLOT virtual void execute() = 0;
    void showBufferDump(void *data);
};
