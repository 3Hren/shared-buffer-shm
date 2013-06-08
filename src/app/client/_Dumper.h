#pragma once

#include "_SharedBufferStorageClient.h"

class SharedBufferReader;
class _Dumper : public _SharedBufferStorageClient
{
    Q_OBJECT
    SharedBufferReader *reader;
public:
    _Dumper(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent = 0);
    ~_Dumper();

protected:
    Q_SLOT void execute();

private:
    void dump();
    void readBuffer();
};
