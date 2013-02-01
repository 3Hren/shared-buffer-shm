#include "Dumper.h"

#include "BufferManager.h"

#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>
#include <QElapsedTimer>

#include <QDebug>

Dumper::Dumper(const QString &name, BufferId buffersCount, BufferSize bufferSize, int timeout, QObject *parent) :
    QObject(parent),
    name(name),
    buffersCount(buffersCount),
    bufferSize(bufferSize),
    timeout(timeout)
{
    shared = new QSharedMemory(name, this);
    manager = new BufferManager(buffersCount, bufferSize);
}

Dumper::~Dumper()
{
    delete manager;
}

void Dumper::execute()
{
    shared->attach(QSharedMemory::ReadOnly);
    qDebug() << "Attached:" << shared->isAttached();
    if (!shared->isAttached()) {
        qCritical() << shared->errorString();
        exit(1);
    }
    QTimer::singleShot(timeout, this, SLOT(dump()));
}

void Dumper::dump()
{
    static int counter = 0;
    counter++;

    QElapsedTimer timer;
    timer.start();
    shared->lock();
    void *data = manager->getBuffersDump(shared->data());
    shared->unlock();

    qDebug() << "#" << counter << ". Dumped" << buffersCount * bufferSize << "values in" << timer.elapsed() << "ms";

//    float *_copy = (float*)copy;
//    for (BufferId id = 0; id < buffersCount; ++id) {
//        QString line;
//        for (BufferSize pos = 0; pos < bufferSize; ++pos) {
//            line += QString::number(_copy[id * bufferSize + pos], 'f', 2).leftJustified(10, ' ');
//        }
//        qDebug() << id << line;
//    }

    delete[] (char*)data;
    QTimer::singleShot(timeout, this, SLOT(dump()));
}
