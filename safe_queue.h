#ifndef safe_queue_h
#define safe_queue_h

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class safe_queue {
public:
    safe_queue() = default;
    safe_queue(const safe_queue&) = delete;
    safe_queue& operator=(const safe_queue&) = delete;

    // записывает в начало очереди новую задачу
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            queue.push(std::move(value));
        }
        cond_var.notify_one();
    }

    // находится в ожидании, пока не придёт уведомление на условную переменную. При нотификации условной переменной данные считываются из очереди
    T pop() {
        std::unique_lock<std::mutex> lock(queue_mutex);
        cond_var.wait(lock, [this] { return !queue.empty() || lstopped; });

        if (queue.empty()) {
            return T{}; // пустая функция — сигнал завершения
        }

        T value = std::move(queue.front());
        queue.pop();
        return value;
    }

    // разбудить все потоки
    void stop() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            lstopped = true;
        }
        cond_var.notify_all();
    }

private:
    std::queue<T> queue; // очередь для хранения задач
    mutable std::mutex queue_mutex; // для реализации блокировки
    std::condition_variable cond_var; // для уведомлений
    bool lstopped{false};
};

#endif