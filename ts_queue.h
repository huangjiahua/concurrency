//
// Created by jiahua on 2019/2/26.
//

#ifndef CONCURRENCY_TS_QUEUE_H
#define CONCURRENCY_TS_QUEUE_H

#include <thread>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <queue>


template <typename T>
class ts_queue {
private:
    std::queue<T> _queue;
    std::mutex _mut;
    std::condition_variable _cond;
public:
    ts_queue() = default;
    ts_queue(const ts_queue&);
    ts_queue& operator=(
            const ts_queue&) = delete;
    void push(T new_value);
    bool try_pop(T& value);
    std::shared_ptr<T> try_pop();
    void wait_and_pop(T& value);
    std::shared_ptr<T> wait_and_pop();
    bool empty() const;
};

template<typename T>
ts_queue<T>::ts_queue(const ts_queue &lhs) {
    std::lock_guard<std::mutex> lk(lhs._mut);
    _queue = lhs._queue;
}

template<typename T>
void ts_queue<T>::push(T new_value) {
    std::lock_guard<std::mutex> lk(_mut);
    _queue.push(new_value);
    _cond.notify_one();
}

template<typename T>
bool ts_queue<T>::try_pop(T &value) {
    std::lock_guard<std::mutex> lk(_mut);
    if (_queue.empty())
        return false;
    value = _queue.front();
    _queue.pop();
    return true;
}

template<typename T>
std::shared_ptr<T> ts_queue<T>::try_pop() {
    std::lock_guard<std::mutex> lk(_mut);
    if (_queue.empty())
        return std::shared_ptr<T>();
    std::shared_ptr<T> ret = std::make_shared<T>(_queue.front);
    _queue.pop();
    return ret;
}

template<typename T>
void ts_queue<T>::wait_and_pop(T &value) {
    std::unique_lock<std::mutex> lk(_mut);
    _cond.wait(lk, [this](){ return !_queue.empty(); });
    value = _queue.front();
    _queue.pop();
}

template<typename T>
std::shared_ptr<T> ts_queue<T>::wait_and_pop() {
    std::unique_lock<std::mutex> lk(_mut);
    _cond.wait(lk, [this](){ return !_queue.empty(); });
    std::shared_ptr<T> ret = std::make_shared<T>(_queue.front());
    _queue.pop();
    return ret;
}

template<typename T>
bool ts_queue<T>::empty() const {
    std::lock_guard<std::mutex> lk(_mut);
    return _queue.empty();
}


#endif //CONCURRENCY_TS_QUEUE_H
