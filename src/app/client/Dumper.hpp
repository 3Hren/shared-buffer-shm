#pragma once

#include "SharedBufferStorageClient.hpp"

class SharedBufferReader;
class _Dumper : public SharedBufferStorageClient
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
