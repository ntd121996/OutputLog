#include <queue>
#include <mutex>
template <typename T>
class Queue
{
private:
    std::queue<T> m_queue;
    std::mutex mutex;
public:
    void Enqueue(T data);
    T Dequeue();
    bool isEmpty();
    int Size();
    void Clear();
    Queue(/* args */);
    ~Queue();
};

template <typename T>
inline void Queue<T>::Enqueue(T data)
{
    std::lock_guard<std::mutex> lock(mutex);
    m_queue.push(data);
}

template <typename T>
inline T Queue<T>::Dequeue()
{
    std::lock_guard<std::mutex> lock(mutex);
    T data;
    if(!m_queue.empty())
    {
        data = std::move(m_queue.front());
        m_queue.pop();
    }
    return data;
}

template <typename T>
inline bool Queue<T>::isEmpty()
{
    std::lock_guard<std::mutex> lock(mutex);
    return m_queue.empty();
}

template <typename T>
inline int Queue<T>::Size()
{
    std::lock_guard<std::mutex> lock(mutex);
    return m_queue.size();
}

template <typename T>
inline void Queue<T>::Clear()
{
    std::lock_guard<std::mutex> lock(mutex);
    std::queue<T>().swap(m_queue);
}
template <typename T>
Queue<T>::Queue(/* args */)
{
}
template <typename T>
Queue<T>::~Queue()
{
}
