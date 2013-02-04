#pragma once

#include "SharedBufferStorageClient.h"

class Pusher : public SharedBufferStorageClient
{
    Q_OBJECT
public:
    Pusher(const QString &name, BufferId buffersCount, BufferSize bufferSize, int timeout, QObject *parent = 0);

protected:
    void execute();

private:
    Q_SLOT void push();
};
