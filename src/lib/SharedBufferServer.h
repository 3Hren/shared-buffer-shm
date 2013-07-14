#pragma once

#include <memory>

#include <QObject>
#include <QSharedMemory>

#include <log4cxx/logger.h>

#include "SharedBufferGlobal.h"

class QTimer;
class LowLevelBufferHandler;
class SharedBufferServer : public QObject
{
    Q_OBJECT
    const QString name;
    const BufferId buffersCount;
    const BufferPos bufferSize;
    QSharedMemory *shared;
    LowLevelBufferHandler *handler;
    QTimer *refreshTimer;
    log4cxx::LoggerPtr log;

    quint32 clientCount = 0;
public:
    SharedBufferServer(const QString &name, BufferId buffersCount, BufferPos bufferSize, QObject *parent = 0);
    ~SharedBufferServer();

    void execute();

private slots:
    void refreshState();
};
