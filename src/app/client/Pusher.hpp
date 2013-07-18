#pragma once

#include <QObject>

#include <log4cxx/logger.h>

#include "SharedBufferWriter.h"
#include "BufferWriter.h"
#include "SharedBufferStorageClient.hpp"

class SharedMemory;
class Pusher : public SharedBufferStorageClient
{
    Q_OBJECT
    SharedBufferWriter *sharedBufferWriter;
    BufferWriter *writer;
    log4cxx::LoggerPtr log;
public:
    Pusher(const QString &name, BufferId buffersCount, BufferPos bufferSize, int timeout, QObject *parent = 0);
    ~Pusher();

protected slots:
    void execute();

private:
    void push();
};
