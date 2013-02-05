#pragma once

#include <QObject>

#include "SharedBufferWriter.h"
#include "BufferWriter.h"

class SharedMemory;
class _Pusher : QObject
{
    Q_OBJECT
    BufferId buffersCount;
    BufferPos bufferSize;
    int timeout;

    LowLevelBufferHandler *lowLevelBufferHandler;
    SharedMemory *sharedMemory;
    SharedBufferWriter *sharedBufferWriter;
    BufferWriter *writer;
public:
    _Pusher(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent = 0);
    ~_Pusher();

    void execute();

private:
    Q_SLOT void push();
};
