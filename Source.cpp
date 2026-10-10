#include "thread_pool.h"

#include <iostream>
#include <chrono>
#include <string>
#include <mutex>

std::mutex my_mutex;

void print_name(const std::string& name) {
    std::lock_guard<std::mutex> lock(my_mutex);
    std::cout << "taskName: " << name << " threadId = " << std::this_thread::get_id() << std::endl;
}

void taskTest1() { print_name("taskTest1"); }
void taskTest2()  { print_name("taskTest2");  }

int main() {
    thread_pool thpool; // число потоков = hardware_concurrency()

    std::cout << "Hardware threads: " << std::thread::hardware_concurrency() << std::endl;

    // Тестовые функции
    thpool.submit(taskTest1);
    thpool.submit(taskTest2);

    thpool.submit([] { print_name("lambdaTest3"); });
    thpool.submit([] { print_name("lambdaTest4"); });

    std::cout << "\n0: submitted 4 tasks" << std::endl;

    for (int i = 1; i < 6; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        thpool.submit([i] { print_name("taskTestA" + std::to_string(i)); });
        thpool.submit([i] { print_name("taskTestB" + std::to_string(i)); });

        std::cout << "\n" << i << ": submitted 2 tasks" << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    thpool.stop();
    std::cout << "\nThread pool stopped." << std::endl;
    return 0;
}
