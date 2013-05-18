#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>
#include <memory>

template<typename T>
class ThreadSafeQueue
{
    mutable std::mutex mutex;
    std::queue<T> queue;
    std::condition_variable queueNotEmptyCondition;
public:
    ThreadSafeQueue() {}

    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(value);
        queueNotEmptyCondition.notify_one();
    }

    std::shared_ptr<T> blockingPop() {
        std::unique_lock<std::mutex> lock(mutex);
        queueNotEmptyCondition.wait(lock, [this]{ return !queue.empty(); });
        std::shared_ptr<T> value(std::make_shared<T>(std::move(queue.front())));
        queue.pop();
        return value;
    }

    bool tryPop(T &value) {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty())
            return false;

        value = std::move(queue.front());
        queue.pop();
        return true;
    }

    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }
};
