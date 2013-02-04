#pragma once

#include "SharedBufferStorageClient.h"

#include "SharedBufferWriter.h"
#include "BufferManager.h"

class _Pusher : public SharedBufferStorageClient
{
    Q_OBJECT
    SharedBufferWriter *s;
    BufferManager *m;
public:
    _Pusher(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent = 0);

protected:
    void execute();

private:
    Q_SLOT void push();
};
