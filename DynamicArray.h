#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

// Custom dynamic array template class to replace std::vector
// Manual memory management with deep copy support
template<typename T>
class DynamicArray {
private:
    T* data;
    int capacity;
    int size;
    
    void resize() {
        capacity = (capacity == 0) ? 1 : capacity * 2;
        T* newData = new T[capacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }
    
public:
    DynamicArray() : data(nullptr), capacity(0), size(0) {}
    
    // Copy constructor for deep copy
    DynamicArray(const DynamicArray& other) : data(nullptr), capacity(0), size(0) {
        if (other.size > 0) {
            capacity = other.capacity;
            size = other.size;
            data = new T[capacity];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
    }
    
    // Assignment operator for deep copy
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            data = nullptr;
            capacity = 0;
            size = 0;
            
            if (other.size > 0) {
                capacity = other.capacity;
                size = other.size;
                data = new T[capacity];
                for (int i = 0; i < size; i++) {
                    data[i] = other.data[i];
                }
            }
        }
        return *this;
    }
    
    ~DynamicArray() {
        delete[] data;
    }
    
    void pushBack(const T& value) {
        if (size >= capacity) {
            resize();
        }
        data[size++] = value;
    }
    
    void removeAt(int index) {
        if (index < 0 || index >= size) return;
        for (int i = index; i < size - 1; i++) {
            data[i] = data[i + 1];
        }
        size--;
    }
    
    void clear() {
        size = 0;
    }
    
    int getSize() const { return size; }
    
    bool isEmpty() const { return size == 0; }
    
    T& operator[](int index) { 
        return data[index]; 
    }
    
    const T& operator[](int index) const { 
        return data[index]; 
    }
    
    // Iterator-like access for compatibility
    T* begin() { return data; }
    T* end() { return data + size; }
    const T* begin() const { return data; }
    const T* end() const { return data + size; }
};

#endif
