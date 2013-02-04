#pragma once

#include "_SharedBufferStorageClient.h"

#include "SharedBufferWriter.h"
#include "BufferWriter.h"

class _Pusher : public _SharedBufferStorageClient
{
    Q_OBJECT
    SharedBufferWriter *sharedBufferWriter;
    BufferWriter *writer;
public:
    _Pusher(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent = 0);

protected:
    void execute();

private:
    Q_SLOT void push();
};
