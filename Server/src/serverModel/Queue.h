//
// Created by Michal on 24.05.2020.
//

#ifndef SERVER_QUEUE_H
#define SERVER_QUEUE_H

#include <mutex>
#include <queue>

template <typename T>
class Queue {

private:
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable emptyQueue;

public:

    void push(T const & elem) {

        std::unique_lock<std::mutex> lock(mutex);

        queue.push(elem);

        if(queue.size() == 1)
            emptyQueue.notify_one();
    }

    const T & pop() {

        std::unique_lock<std::mutex> lock(mutex);

        while(queue.empty())
            emptyQueue.wait(lock);

        T & value = queue.front();
        queue.pop();
        return value;
    }

    const int & size() {
        std::unique_lock<std::mutex> lock(mutex);

        return queue.size();
    }

    const T & front() {
        std::unique_lock<std::mutex> lock(mutex);

        return queue.front();
    }
};

#endif //SERVER_QUEUE_H
