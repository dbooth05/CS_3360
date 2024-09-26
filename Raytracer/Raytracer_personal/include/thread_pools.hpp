#ifndef THREAD_POOLS_HPP
#define THREAD_POOLS_HPP

#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

using std::thread;
using std::function;
using std::vector;
using std::queue;

class ThreadPool {
    public:
        ThreadPool(size_t num = thread::hardware_concurrency());

        ~ThreadPool();

        void enqueue(function<void()> task);

    private:

        vector<thread> threads;

        queue<function<void()>> tasks;

        std::mutex queue_mutex;

        std::condition_variable cv;        

        bool stop = false;

};

#endif