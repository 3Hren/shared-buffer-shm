#pragma once

#include <QObject>
#include "SharedBufferGlobal.h"

#include <QSharedMemory>

class BufferManager;
class Dumper : public QObject
{
    Q_OBJECT
    QString name;
    BufferId buffersCount;
    BufferSize bufferSize;
    int timeout;
    BufferManager *manager;
    QSharedMemory *shared;
public:
    Dumper(const QString &name, BufferId buffersCount, BufferSize bufferSize, int timeout, QObject *parent = 0);
    ~Dumper();
    void execute();

private:
    Q_SLOT void dump();
};
