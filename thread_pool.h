#ifndef thread_pool_h
#define thread_pool_h

#include "safe_queue.h"

#include <vector>
#include <thread>
#include <atomic>
#include <functional>

class thread_pool {
public:
    explicit thread_pool(size_t threads = std::thread::hardware_concurrency());
    ~thread_pool();

    thread_pool(const thread_pool& thp) = delete;
    thread_pool& operator=(const thread_pool& thp) = delete;

    // помещает в очередь задачу
    void submit(std::function<void()> task);

    // останавливает пул потоков
    void stop();

private:
    // выбирает из очереди очередную задачу и исполняет её
    void work();

private:    
    std::vector<std::thread> workers;
    safe_queue<std::function<void()>> queue;
    std::atomic<bool> stop_called;
};

#endif