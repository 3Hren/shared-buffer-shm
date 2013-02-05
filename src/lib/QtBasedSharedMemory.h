#pragma once

#include "SharedMemory.h"

class QSharedMemory;
class QtBasedSharedMemory : public SharedMemory
{
    QSharedMemory *sharedMemory;
public:
    QtBasedSharedMemory();
    ~QtBasedSharedMemory();

    bool create(int size, AccessMode mode = AccessMode::ReadWrite);
    int size() const;
    bool attach(AccessMode mode = AccessMode::ReadWrite);
    bool isAttached() const;

    const void *constData() const;
    void *data();

    void setKey(const QString &key);
    QString getKey() const;

    bool lock();
    bool unlock();

    QString getErrorDescription() const;
};
