#pragma once

#include <QObject>

#include "SharedBufferGlobal.h"

class SharedMemory;
class LowLevelBufferHandler;
class SharedBufferStorageClient : public QObject
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
    SharedBufferStorageClient(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent = 0);
    ~SharedBufferStorageClient();

    void connectAndRun();

protected slots:
    virtual void execute() = 0;
};
