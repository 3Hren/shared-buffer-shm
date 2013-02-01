#pragma once

#include <QObject>
#include "SharedBufferGlobal.h"

#include <QSharedMemory>

class Server : public QObject
{
    Q_OBJECT
    QString name;
    BufferId buffersCount;
    BufferSize bufferSize;
    QSharedMemory *shared;
public:
    Server(const QString &name, BufferId buffersCount, BufferSize bufferSize, QObject *parent = 0);
    Q_SLOT void execute();
};
