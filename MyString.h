#ifndef MYSTRING_H
#define MYSTRING_H

#include <iostream>

class MyString {
private:
    char* buffer;
    int len;
    int cap;

    void copyFrom(const char* src);
    void resize(int newCap);

public:
    MyString();
    MyString(const char* src);
    MyString(const MyString& other);
    ~MyString();

    MyString& operator=(const char* src);
    MyString& operator=(const MyString& other);

    bool operator==(const MyString& other) const;
    bool operator==(const char* other) const;
    bool operator!=(const MyString& other) const;
    bool operator!=(const char* other) const;

    MyString operator+(const MyString& other) const;
    MyString& operator+=(const MyString& other);
    MyString& operator+=(const char* src);

    char operator[](int index) const;

    friend std::ostream& operator<<(std::ostream& os, const MyString& str);
    friend std::istream& operator>>(std::istream& is, MyString& str);

    int length() const;
    int capacity() const;
    const char* c_str() const;
    void clear();
    char charAt(int i) const;
    MyString toLower() const;
    bool equals(const char* other) const;
    int findChar(char c) const;
    MyString substr(int start, int length) const;
    int toInt() const;
    float toFloat() const;
    bool isEmpty() const;
};

#endif
