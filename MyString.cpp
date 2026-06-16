#include "MyString.h"

void MyString::copyFrom(const char* src) {
    int srcLen = 0;
    while (src[srcLen] != '\0') srcLen++;
    cap = 8;
    while (cap < srcLen + 1) cap *= 2;
    buffer = new char[cap];
    for (int i = 0; i < srcLen; i++) {
        buffer[i] = src[i];
    }
    buffer[srcLen] = '\0';
    len = srcLen;
}

void MyString::resize(int newCap) {
    int nc = cap;
    while (nc < newCap) nc *= 2;
    char* newBuffer = new char[nc];
    for (int i = 0; i <= len; i++) {
        newBuffer[i] = buffer[i];
    }
    delete[] buffer;
    buffer = newBuffer;
    cap = nc;
}

MyString::MyString() {
    cap = 8;
    len = 0;
    buffer = new char[cap];
    buffer[0] = '\0';
}

MyString::MyString(const char* src) {
    copyFrom(src);
}

MyString::MyString(const MyString& other) {
    copyFrom(other.buffer);
}

MyString::~MyString() {
    delete[] buffer;
}

MyString& MyString::operator=(const char* src) {
    delete[] buffer;
    copyFrom(src);
    return *this;
}

MyString& MyString::operator=(const MyString& other) {
    if (this == &other) return *this;
    delete[] buffer;
    copyFrom(other.buffer);
    return *this;
}

bool MyString::operator==(const MyString& other) const {
    if (len != other.len) return false;
    for (int i = 0; i < len; i++) {
        if (buffer[i] != other.buffer[i]) return false;
    }
    return true;
}

bool MyString::operator==(const char* other) const {
    int i = 0;
    while (buffer[i] != '\0' && other[i] != '\0') {
        if (buffer[i] != other[i]) return false;
        i++;
    }
    return buffer[i] == other[i];
}

bool MyString::operator!=(const MyString& other) const {
    return !((*this) == other);
}

bool MyString::operator!=(const char* other) const {
    return !((*this) == other);
}

MyString MyString::operator+(const MyString& other) const {
    MyString result(buffer);
    result += other;
    return result;
}

MyString& MyString::operator+=(const MyString& other) {
    return (*this) += other.buffer;
}

MyString& MyString::operator+=(const char* src) {
    int srcLen = 0;
    while (src[srcLen] != '\0') srcLen++;
    int newLen = len + srcLen;
    if (newLen + 1 > cap) resize(newLen + 1);
    for (int i = 0; i < srcLen; i++) {
        buffer[len + i] = src[i];
    }
    len = newLen;
    buffer[len] = '\0';
    return *this;
}

char MyString::operator[](int index) const {
    if (index < 0 || index >= len) return '\0';
    return buffer[index];
}

std::ostream& operator<<(std::ostream& os, const MyString& str) {
    os << str.buffer;
    return os;
}

std::istream& operator>>(std::istream& is, MyString& str) {
    char ch;
    str.clear();
    bool started = false;
    while (is.get(ch)) {
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
            if (!started) continue;
            else break;
        }
        started = true;
        char temp[2];
        temp[0] = ch;
        temp[1] = '\0';
        str += temp;
    }
    return is;
}

int MyString::length() const {
    return len;
}

int MyString::capacity() const {
    return cap;
}

const char* MyString::c_str() const {
    return buffer;
}

void MyString::clear() {
    len = 0;
    buffer[0] = '\0';
}

char MyString::charAt(int i) const {
    if (i < 0 || i >= len) return '\0';
    return buffer[i];
}

MyString MyString::toLower() const {
    MyString result(buffer);
    for (int i = 0; i < result.len; i++) {
        char c = result.buffer[i];
        if (c >= 'A' && c <= 'Z') {
            result.buffer[i] = c + 32;
        }
    }
    return result;
}

bool MyString::equals(const char* other) const {
    return (*this) == other;
}

int MyString::findChar(char c) const {
    for (int i = 0; i < len; i++) {
        if (buffer[i] == c) return i;
    }
    return -1;
}

MyString MyString::substr(int start, int length) const {
    MyString result;
    if (start < 0 || start > len) return result;
    int actualLen = length;
    if (start + actualLen > len) actualLen = len - start;
    if (actualLen < 0) actualLen = 0;
    char temp[2];
    temp[1] = '\0';
    for (int i = 0; i < actualLen; i++) {
        temp[0] = buffer[start + i];
        result += temp;
    }
    return result;
}

int MyString::toInt() const {
    int i = 0;
    bool negative = false;
    if (len > 0 && buffer[0] == '-') {
        negative = true;
        i = 1;
    }
    int result = 0;
    while (i < len && buffer[i] >= '0' && buffer[i] <= '9') {
        result = result * 10 + (buffer[i] - '0');
        i++;
    }
    if (negative) result = -result;
    return result;
}

float MyString::toFloat() const {
    int i = 0;
    bool negative = false;
    if (len > 0 && buffer[0] == '-') {
        negative = true;
        i = 1;
    }
    float result = 0.0f;
    while (i < len && buffer[i] >= '0' && buffer[i] <= '9') {
        result = result * 10.0f + (buffer[i] - '0');
        i++;
    }
    if (i < len && buffer[i] == '.') {
        i++;
        float frac = 0.1f;
        while (i < len && buffer[i] >= '0' && buffer[i] <= '9') {
            result += (buffer[i] - '0') * frac;
            frac *= 0.1f;
            i++;
        }
    }
    if (negative) result = -result;
    return result;
}

bool MyString::isEmpty() const {
    return len == 0;
}
