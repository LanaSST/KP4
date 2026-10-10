#include "safe_queue.h"
#include "thread_pool.h"

thread_pool::thread_pool(size_t threads)
    : stop_called(false)
{
    if (threads == 0) {
        threads = 1;
    }

    for (size_t i = 0; i < threads; ++i) {
        // передаём work конструктору потока
        workers.emplace_back(&thread_pool::work, this);
    }
}

thread_pool::~thread_pool() {
    stop();
}

void thread_pool::submit(std::function<void()> task) {
    queue.push(std::move(task));
}

void thread_pool::stop() {
    bool expected = false;
    if (!stop_called.compare_exchange_strong(expected, true)) {
        return;
    }

    queue.stop();

    for (auto& t : workers) {
        if (t.joinable()) {
            t.join();
        }
    }
    workers.clear();
}

void thread_pool::work() {
    while (true) {
        auto task = queue.pop();
        if (!task) {
            break;
        }
        task();
    }
}


