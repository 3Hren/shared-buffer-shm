#include "Pusher.h"

#include "BufferManager.h"

#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>
#include <QElapsedTimer>

#include <QDebug>

Pusher::Pusher(const QString &name, BufferId buffersCount, BufferSize bufferSize, int timeout, QObject *parent) :
    QObject(parent),
    name(name),
    buffersCount(buffersCount),
    bufferSize(bufferSize),
    timeout(timeout)
{
    shared = new QSharedMemory(name, this);
    manager = new BufferManager(buffersCount, bufferSize);
}

Pusher::~Pusher()
{
    delete manager;
}

void Pusher::execute()
{
    shared->attach();
    qDebug() << "Attached:" << shared->isAttached();
    if (!shared->isAttached()) {
        qCritical() << shared->errorString();
        exit(1);
    }
    QTimer::singleShot(timeout, this, SLOT(push()));
}

void Pusher::push()
{
    static int counter = 0;
    counter++;
    SignalValue *data = new SignalValue[buffersCount];        
    for (int i = 0; i < buffersCount; ++i)
        data[i] = counter;

    QElapsedTimer timer;
    timer.start();
    shared->lock();
    manager->push(QDateTime::currentDateTime().toMSecsSinceEpoch(), data, shared->data());

//    float *to = (float*)shared->data();
//    for (BufferId id = 0; id < buffersCount; ++id) {
//        QString line;
//        for (BufferSize pos = 0; pos < bufferSize; ++pos) {
//            line += QString::number(to[id * bufferSize + pos], 'f', 2).leftJustified(10, ' ');
//        }
//        qDebug() << id << line;
//    }

    shared->unlock();
    delete[] data;

    qDebug() << "#" << counter << ". Pushed" << buffersCount << "values in" << timer.elapsed() << "ms";
    QTimer::singleShot(timeout, this, SLOT(push()));
}
