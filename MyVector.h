#ifndef MYVECTOR_H
#define MYVECTOR_H

#include "InvalidInputException.h"

template <typename T>
class MyVector {
private:
    T* data;
    int count;
    int cap;

    void resize() {
        int newCap = cap * 2;
        T* newData = new T[newCap];
        for (int i = 0; i < count; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        cap = newCap;
    }

public:
    MyVector() {
        cap = 4;
        count = 0;
        data = new T[cap];
    }

    MyVector(const MyVector<T>& other) {
        cap = other.cap;
        count = other.count;
        data = new T[cap];
        for (int i = 0; i < count; i++) {
            data[i] = other.data[i];
        }
    }

    MyVector<T>& operator=(const MyVector<T>& other) {
        if (this == &other) return *this;
        delete[] data;
        cap = other.cap;
        count = other.count;
        data = new T[cap];
        for (int i = 0; i < count; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }

    ~MyVector() {
        delete[] data;
    }

    void add(const T& item) {
        if (count == cap) resize();
        data[count] = item;
        count++;
    }

    void removeAt(int index) {
        if (index < 0 || index >= count) return;
        for (int i = index; i < count - 1; i++) {
            data[i] = data[i + 1];
        }
        count--;
    }

    T& operator[](int index) {
        if (index < 0 || index >= count) {
            throw InvalidInputException("Index out of range");
        }
        return data[index];
    }

    int size() const {
        return count;
    }

    void clear() {
        count = 0;
    }

    T& get(int index) {
        return (*this)[index];
    }
};

#endif
