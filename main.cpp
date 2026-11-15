#include <iostream>
#include "ThreadSafeLRUCache.h"
#include "ThreadPool.h"

int main() {
    // Initialize LRU Cache with capacity of 3
    ThreadSafeLRUCache<int, std::string> lru(3);
    
    // Initialize Thread Pool with 4 worker threads
    ThreadPool pool(4);

    // Enqueue tasks that write and read from the cache concurrently
    auto future1 = pool.enqueue([&lru]() {
        lru.put(1, "Alpha");
        lru.put(2, "Beta");
        return "Task 1 completed";
    });

    auto future2 = pool.enqueue([&lru]() {
        lru.put(3, "Gamma");
        lru.put(4, "Delta"); // This should evict key 1 (Alpha)
        return "Task 2 completed";
    });

    // Wait for tasks to finish
    std::cout << future1.get() << std::endl;
    std::cout << future2.get() << std::endl;

    // Verify cache contents
    auto val1 = lru.get(1);
    auto val4 = lru.get(4);

    std::cout << "Key 1 present: " << (val1.has_value() ? "Yes" : "No (Evicted correctly)") << std::endl;
    std::cout << "Key 4 value: " << (val4.has_value() ? val4.value() : "Not found") << std::endl;

    return 0;
}