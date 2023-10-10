#ifndef BAG_H
#define BAG_H

#include <iostream>
#include <cstring>
#include <stdexcept>

template <class T>
class Bag {
    protected:
    T* array;
    int capacity;
    int top_ptr = -1;

    void change_size(int new_capacity) {
        T* tmp_array = new T[new_capacity];
        const int min_size = (capacity < new_capacity ? capacity : new_capacity);
        std::memcpy(tmp_array, array, min_size * sizeof(T));
        delete[] array;
        array = tmp_array;
        capacity = new_capacity;
    }

    public:
    Bag(int capacity = 10) {
        if (capacity < 1) throw std::range_error("Capacity must be > 0");
        array = new T[capacity];
        this->capacity = capacity;
    }

    Bag(const Bag& item): capacity(item.capacity), top_ptr(item.top_ptr) {
        array = new T[item.capacity];
        std::memcpy(array, item.array, item.capacity * sizeof(T));
    }

    Bag& operator=(const Bag& item) {
        if (this != &item) {
            delete[] array;
            array = new T[item.capacity];
            std::memcpy(array, item.array, item.capacity * sizeof(T));
            capacity = item.capacity;
            top_ptr = item.top_ptr;
        }

        return *this;
        
    }

    ~Bag() {
        delete[] array;
    }

    virtual void clear() {
        delete array;
        array = new T[capacity];
        int top_ptr = -1;
    }

    virtual int size() const {
        return top_ptr + 1;
    }

    virtual bool is_empty() const {
        return (top_ptr + 1 == 0);
    }

    virtual const T& element() const {
        return array[0];
    }

    virtual void push(const T& item) {
        if (capacity == top_ptr + 1) {
            change_size(capacity * 2);
        }

        array[top_ptr + 1] = item;

        top_ptr++;
    }

    virtual void pop() {
        if (is_empty()) throw std::range_error("Bag is empty, cannot delete");

        array[top_ptr].~T();
        top_ptr--;

        // compact array
        if (size() < capacity / 4) {
            change_size(capacity / 4);
        }
    }
};

#endif