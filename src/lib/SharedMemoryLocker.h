#pragma once

template<typename T>
class SharedMemoryLocker {
    T *sharedMemory;
public:
    SharedMemoryLocker(T *sharedMemory) :
        sharedMemory(sharedMemory)
    {
        lock();
    }

    virtual ~SharedMemoryLocker() {
        unlock();
    }

    void lock() {
        sharedMemory->lock();
    }

    void unlock() {
        sharedMemory->unlock();
    }
};
