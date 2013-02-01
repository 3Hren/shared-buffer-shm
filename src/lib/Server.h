#pragma once

#include <QObject>
#include "SharedBufferGlobal.h"

#include <QSharedMemory>

class Server : public QObject
{
    Q_OBJECT
    const QString name;
    const BufferId buffersCount;
    const BufferSize bufferSize;
    QSharedMemory *shared;
public:
    Server(const QString &name, BufferId buffersCount, BufferSize bufferSize, QObject *parent = 0);

    void execute();
};
