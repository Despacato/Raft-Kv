#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T> // 模板类只能类内实现
class LockQueue
{
public:
    void Push(const T &value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(value);
        m_cond.notify_one();
    }

    T Pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_queue.empty())
        {
            m_cond.wait(lock);
        }
        T value = m_queue.front();
        m_queue.pop();
        return value;
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cond;
};