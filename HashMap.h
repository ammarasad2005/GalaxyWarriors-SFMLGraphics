#ifndef HASHMAP_H
#define HASHMAP_H

#include <string>
#include <type_traits>

// Custom hash map implementation to replace std::map
// Uses chaining for collision resolution
template<typename K, typename V>
class HashMap {
private:
    struct Entry {
        K key;
        V value;
        Entry* next;
        
        Entry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };
    
    Entry** buckets;
    int bucketCount;
    int itemCount;
    
    // Hash function
    int hash(const K& key) const {
        // For string types
        if constexpr (std::is_same<K, std::string>::value) {
            unsigned long hashValue = 5381;
            for (char c : key) {
                hashValue = ((hashValue << 5) + hashValue) + c;
            }
            return hashValue % bucketCount;
        } else {
            // Generic hash for other types
            return reinterpret_cast<unsigned long>(&key) % bucketCount;
        }
    }
    
public:
    HashMap(int initialCapacity = 16) 
        : bucketCount(initialCapacity), itemCount(0) {
        buckets = new Entry*[bucketCount];
        for (int i = 0; i < bucketCount; i++) {
            buckets[i] = nullptr;
        }
    }
    
    ~HashMap() {
        clear();
        delete[] buckets;
    }
    
    void insert(const K& key, const V& value) {
        int index = hash(key);
        Entry* entry = buckets[index];
        
        // Check if key already exists
        while (entry != nullptr) {
            if (entry->key == key) {
                entry->value = value;
                return;
            }
            entry = entry->next;
        }
        
        // Insert new entry at the beginning
        Entry* newEntry = new Entry(key, value);
        newEntry->next = buckets[index];
        buckets[index] = newEntry;
        itemCount++;
    }
    
    V* find(const K& key) const {
        int index = hash(key);
        Entry* entry = buckets[index];
        
        while (entry != nullptr) {
            if (entry->key == key) {
                return &(entry->value);
            }
            entry = entry->next;
        }
        
        return nullptr;
    }
    
    bool contains(const K& key) const {
        int index = hash(key);
        Entry* entry = buckets[index];
        
        while (entry != nullptr) {
            if (entry->key == key) {
                return true;
            }
            entry = entry->next;
        }
        
        return false;
    }
    
    V& operator[](const K& key) {
        V* found = find(key);
        if (found != nullptr) {
            return *found;
        }
        
        // Insert default value if key doesn't exist
        V defaultValue = V();
        insert(key, defaultValue);
        return *find(key);
    }
    
    void remove(const K& key) {
        int index = hash(key);
        Entry* entry = buckets[index];
        Entry* prev = nullptr;
        
        while (entry != nullptr) {
            if (entry->key == key) {
                if (prev == nullptr) {
                    buckets[index] = entry->next;
                } else {
                    prev->next = entry->next;
                }
                delete entry;
                itemCount--;
                return;
            }
            prev = entry;
            entry = entry->next;
        }
    }
    
    void clear() {
        for (int i = 0; i < bucketCount; i++) {
            Entry* entry = buckets[i];
            while (entry != nullptr) {
                Entry* next = entry->next;
                delete entry;
                entry = next;
            }
            buckets[i] = nullptr;
        }
        itemCount = 0;
    }
    
    int getSize() const {
        return itemCount;
    }
    
    bool isEmpty() const {
        return itemCount == 0;
    }
};

#endif
