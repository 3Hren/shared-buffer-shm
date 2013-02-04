#pragma once

#include "_SharedBufferStorageClient.h"

class _Dumper : public _SharedBufferStorageClient
{
    Q_OBJECT
public:
    _Dumper(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent = 0);

protected:
    void execute();

private:
    Q_SLOT void dump();
};
