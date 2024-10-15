// #include "thread_pools.hpp"

// ThreadPool::ThreadPool(size_t num = thread::hardware_concurrency()) {
    
//     for (size_t i = 0; i < num; ++i) {
//         threads.emplace_back([this] {
//             while (true) {
//                 function<void()> task;

//                 {
//                     std::unique_lock<std::mutex> lock(queue_mutex);

//                     cv.wait(lock, [this] {
//                         return !tasks.empty() || stop;
//                     }); 

//                     if (stop && tasks.empty()) {
//                         return;
//                     }

//                     task = move(tasks.front());
//                     tasks.pop();
//                 }

//                 task();
//             }
//         });
//     }
// }

// ThreadPool::~ThreadPool() {
//     {
//         std::unique_lock<std::mutex> lock(queue_mutex);
//         stop = true;
//     }

//     cv.notify_all();

//     for (auto &thread : threads) {
//         thread.join();
//     }
// }

// void ThreadPool::enqueue(function<void()> task) {
//     {
//         std::unique_lock<std::mutex> lock(queue_mutex);
//         tasks.emplace(move(task));
//     }

//     cv.notify_one();
// }