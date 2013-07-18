#pragma once

#include "SharedBufferStorageClient.hpp"

class SharedBufferReader;
class Dumper : public SharedBufferStorageClient
{
    Q_OBJECT
    SharedBufferReader *reader;
public:
    Dumper(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent = 0);
    ~Dumper();

protected:
    Q_SLOT void execute();

private:
    void dump();
    void readBuffer();
};
