#pragma once

#include <QObject>
#include "SharedBufferGlobal.h"

class QSharedMemory;
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
    QSharedMemory *shared;
public:
    _SharedBufferStorageClient(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent = 0);
    ~_SharedBufferStorageClient();

    void connectAndRun();

protected:
    Q_SLOT virtual void execute() = 0;
};
