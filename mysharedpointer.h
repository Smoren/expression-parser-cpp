#pragma once

#include <iostream>


template <typename T> class MySharedPointer {
public:
    MySharedPointer(T* ptr) : ptr(ptr) {
        usageCounter = new unsigned long(1);
        std::cout << *ptr << " | constructor | links: " << *usageCounter << " | (created)" << std::endl;
    }

    MySharedPointer(const MySharedPointer& shPtr) {
        this->ptr = shPtr.ptr;
        usageCounter = shPtr.usageCounter;
        ++(*usageCounter);
        std::cout << *ptr << " | constructor | links: " << *usageCounter << " | (copied)" << std::endl;
    }

    ~MySharedPointer() {
        std::cout << *ptr << " | destructor  | links: " << (*usageCounter-1) << " | ";
        if(--(*usageCounter) == 0) {
            std::cout << "(removed)";
            delete usageCounter;
            delete ptr;
        } else {
            std::cout << "(not removed)";
        }
        std::cout << std::endl;
    }

    T* get() {
        return ptr;
    }
private:
    T* ptr;
    unsigned long* usageCounter;
};

