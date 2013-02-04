#pragma once

#include "SharedBufferStorageClient.h"

class Dumper : public SharedBufferStorageClient
{
    Q_OBJECT
public:
    Dumper(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent = 0);

protected:
    void execute();

private:
    Q_SLOT void dump();
};
