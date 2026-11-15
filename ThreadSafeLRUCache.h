#pragma once
#include <unordered_map>
#include <list>
#include <mutex>
#include <optional>

template <typename Key, typename Value>
class ThreadSafeLRUCache {
private:
    size_t capacity;
    std::list<std::pair<Key, Value>> cacheList; // Stores key-value pairs; most recent at front
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> cacheMap; // O(1) lookup
    mutable std::mutex mtx; // Protects shared state across threads

public:
    explicit ThreadSafeLRUCache(size_t cap) : capacity(cap) {}

    // Put or update a value in the cache
    void put(const Key& key, const Value& value) {
        std::lock_guard<std::mutex> lock(mtx);

        // 1. If key already exists, update value and move to front
        auto it = cacheMap.find(key);
        if (it != cacheMap.end()) {
            it->second->second = value;
            cacheList.splice(cacheList.begin(), cacheList, it->second);
            return;
        }

        // 2. If capacity reached, evict the least recently used item (at the back)
        if (cacheList.size() >= capacity) {
            auto last = cacheList.back();
            cacheMap.erase(last.first);
            cacheList.pop_back();
        }

        // 3. Insert new item at the front
        cacheList.emplace_front(key, value);
        cacheMap[key] = cacheList.begin();
    }

    // Get a value from the cache
    std::optional<Value> get(const Key& key) {
        std::lock_guard<std::mutex> lock(mtx);

        auto it = cacheMap.find(key);
        if (it == cacheMap.end()) {
            return std::nullopt; // Cache miss
        }

        // Move accessed item to the front (marking it most recently used)
        cacheList.splice(cacheList.begin(), cacheList, it->second);
        return it->second->second;
    }
};