#ifndef __THREADSAFE_QUEUE_H__
#define __THREADSAFE_QUEUE_H__
#include "message_listener.h"

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <semaphore>

template <typename T>
class ThreadsafeQueue
{
public:
    ThreadsafeQueue(std::string name)
        : queueName{name}
    {
    }
    void Push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        Notify(new_value);
        data_queue.push(std::move(new_value));
        data_cond.notify_one();
    }
    std::shared_ptr<T> Pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }
    size_t Size() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.size();
    }
    void WaitPop(T &value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]
                       { return !data_queue.empty(); });
        value = std::move(data_queue.front());
        data_queue.pop();
    }
    std::shared_ptr<T> WaitPop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]
                       { return !data_queue.empty(); });
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }
    bool TryPop(T &value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = std::move(data_queue.front());
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> TryPop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }
    bool IsEmpty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }

private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::string queueName;
    std::condition_variable data_cond;
};

#endif