#pragma once

#include <QString>

#include "domain/Memory.hpp"

class SharedMemory
{
public:
    virtual ~SharedMemory() {}

    virtual bool create(int size, AccessMode mode = AccessMode::ReadWrite) = 0;
    virtual int size() const = 0;
    virtual bool attach(AccessMode mode = AccessMode::ReadWrite) = 0;
    virtual bool isAttached() const = 0;

    virtual const void *constData() const = 0;
    virtual void *data() = 0;

    virtual void setKey(const QString &key) = 0;
    virtual QString getKey() const = 0;

    virtual bool lock() = 0;
    virtual bool unlock() = 0;

    virtual QString getErrorDescription() const = 0;
};
