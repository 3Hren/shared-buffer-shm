#pragma once

#include <QObject>
#include "SharedBufferGlobal.h"

#include <QSharedMemory>

class BufferManager;
class Pusher : public QObject
{
    Q_OBJECT
    QString name;
    BufferId buffersCount;
    BufferSize bufferSize;
    int timeout;
    BufferManager *manager;
    QSharedMemory *shared;
public:
    Pusher(const QString &name, BufferId buffersCount, BufferSize bufferSize, int timeout, QObject *parent = 0);
    ~Pusher();
    void execute();

private:
    Q_SLOT void push();
};
