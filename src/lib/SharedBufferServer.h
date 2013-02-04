#pragma once

#include <QObject>
#include "SharedBufferGlobal.h"

#include <QSharedMemory>

class SharedBufferServer : public QObject
{
    Q_OBJECT
    const QString name;
    const BufferId buffersCount;
    const BufferPos bufferSize;
    QSharedMemory *shared;
public:
    SharedBufferServer(const QString &name, BufferId buffersCount, BufferPos bufferSize, QObject *parent = 0);

    void execute();
};
