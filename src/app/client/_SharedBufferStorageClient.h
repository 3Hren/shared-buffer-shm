#pragma once

#include <QObject>
#include "SharedBufferGlobal.h"

class SharedMemory;
class LowLevelBufferHandler;
class _SharedBufferStorageClient : public QObject
{
    Q_OBJECT
protected:
    QString name;
    BufferId buffersCount;
    BufferPos bufferSize;
    int timeout;
    LowLevelBufferHandler *manager;
    SharedMemory *shared;
public:
    _SharedBufferStorageClient(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent = 0);
    ~_SharedBufferStorageClient();

    void connectAndRun();

protected:
    Q_SLOT virtual void execute() = 0;
};
