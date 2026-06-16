#include "Validator.h"
#include <ctime>

bool Validator::validateId(const MyString& id) {
    if (id.isEmpty()) return false;
    for (int i = 0; i < id.length(); i++) {
        char c = id.charAt(i);
        if (c < '0' || c > '9') return false;
    }
    return id.toInt() > 0;
}

bool Validator::validateDate(const MyString& date) {
    if (date.length() != 10) return false;
    for (int i = 0; i < 10; i++) {
        char c = date.charAt(i);
        if (i == 2 || i == 5) {
            if (c != '-') return false;
        } else {
            if (c < '0' || c > '9') return false;
        }
    }
    int day = date.substr(0, 2).toInt();
    int month = date.substr(3, 2).toInt();
    int year = date.substr(6, 4).toInt();
    if (day < 1 || day > 31) return false;
    if (month < 1 || month > 12) return false;
    time_t now = time(nullptr);
    tm localTime;
    localtime_s(&localTime, &now);
    int currentYear = localTime.tm_year + 1900;
    if (year < currentYear) return false;
    return true;
}

bool Validator::validateTimeSlot(const MyString& slot) {
    if (slot == "09:00") return true;
    if (slot == "10:00") return true;
    if (slot == "11:00") return true;
    if (slot == "12:00") return true;
    if (slot == "13:00") return true;
    if (slot == "14:00") return true;
    if (slot == "15:00") return true;
    if (slot == "16:00") return true;
    return false;
}

bool Validator::validateContact(const MyString& contact) {
    if (contact.length() != 11) return false;
    for (int i = 0; i < 11; i++) {
        char c = contact.charAt(i);
        if (c < '0' || c > '9') return false;
    }
    return true;
}

bool Validator::validatePassword(const MyString& password) {
    return password.length() >= 6;
}

bool Validator::validatePositiveFloat(const MyString& value) {
    if (value.isEmpty()) return false;
    int dotCount = 0;
    for (int i = 0; i < value.length(); i++) {
        char c = value.charAt(i);
        if (c == '.') {
            dotCount++;
            if (dotCount > 1) return false;
        } else if (c < '0' || c > '9') {
            return false;
        }
    }
    return value.toFloat() > 0.0f;
}

bool Validator::validateMenuChoice(const MyString& choice, int min, int max) {
    if (choice.isEmpty()) return false;
    for (int i = 0; i < choice.length(); i++) {
        char c = choice.charAt(i);
        if (c < '0' || c > '9') return false;
    }
    int val = choice.toInt();
    return val >= min && val <= max;
}

bool Validator::caseInsensitiveEqual(const MyString& a, const MyString& b) {
    MyString lowerA = a.toLower();
    MyString lowerB = b.toLower();
    return lowerA == lowerB;
}
