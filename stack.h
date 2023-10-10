#ifndef STACK_H
#define STACK_H

#include <stdexcept>

#include "bag.h"

template <class T>
class Stack: public Bag<T> {
    public:
    Stack(int capacity = 10): Bag<T>::Bag(capacity) {}

    const T& top() const {
        if (Bag<T>::is_empty()) throw std::range_error("Stack is empty.");
        return (this->array)[this->top_ptr];
    }

    void pop() {
        if (Bag<T>::is_empty()) throw std::range_error("Stack is empty. Cannot delete.");
        
        (this->top_ptr)--;
    }
};

#endif