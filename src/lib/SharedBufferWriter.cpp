#include "SharedBufferWriter.h"

#include "LowLevelBufferHandler.h"
#include "SharedBufferException.h"

#include <QSharedMemory>
#include <QDebug>

SharedBufferWriter::SharedBufferWriter(LowLevelBufferHandler *manager) :
    manager(manager)
{
    shared = new QSharedMemory;    
}

SharedBufferWriter::~SharedBufferWriter()
{
    delete shared;
}

BufferId SharedBufferWriter::getBuffersCount() const
{
    return manager->getBuffersCount();
}

void SharedBufferWriter::attach(const QString &key)
{
    if (shared->isAttached())
        throw SharedBufferAlreadyAttachedException();

    shared->setKey(key);
    if (!shared->attach())
        throw SharedBufferNotAttachedException(shared->errorString());    
}

void SharedBufferWriter::push(TimeStamp timestamp, const SignalValue *signalsPack) const
{
    if (!shared->isAttached())
        throw SharedBufferNotAttachedException(shared->errorString());

    shared->lock();
    manager->push(timestamp, signalsPack, shared->data());
    shared->unlock();
}
