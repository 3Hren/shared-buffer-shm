#include "QtBasedSharedMemory.h"

#include <QSharedMemory>

QtBasedSharedMemory::QtBasedSharedMemory() :
    sharedMemory(new QSharedMemory)
{    
}

QtBasedSharedMemory::~QtBasedSharedMemory()
{
    delete sharedMemory;
}

bool QtBasedSharedMemory::create(int size, SharedMemory::AccessMode mode)
{
    return sharedMemory->create(size, static_cast<QSharedMemory::AccessMode>(mode));
}

int QtBasedSharedMemory::size() const
{
    return sharedMemory->size();
}

bool QtBasedSharedMemory::attach(SharedMemory::AccessMode mode)
{
    return sharedMemory->attach(static_cast<QSharedMemory::AccessMode>(mode));
}

bool QtBasedSharedMemory::isAttached() const
{
    return sharedMemory->isAttached();
}

const void *QtBasedSharedMemory::constData() const
{
    return sharedMemory->constData();
}

void *QtBasedSharedMemory::data()
{
    return sharedMemory->data();
}

void QtBasedSharedMemory::setKey(const QString &key)
{
    sharedMemory->setKey(key);
}

QString QtBasedSharedMemory::getKey() const
{
    return sharedMemory->key();
}

bool QtBasedSharedMemory::lock()
{
    return sharedMemory->lock();
}

bool QtBasedSharedMemory::unlock()
{
    return sharedMemory->unlock();
}

QString QtBasedSharedMemory::getErrorDescription() const
{
    return sharedMemory->errorString();
}
