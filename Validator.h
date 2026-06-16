#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "MyString.h"

class Validator {
public:
    static bool validateId(const MyString& id);
    static bool validateDate(const MyString& date);
    static bool validateTimeSlot(const MyString& slot);
    static bool validateContact(const MyString& contact);
    static bool validatePassword(const MyString& password);
    static bool validatePositiveFloat(const MyString& value);
    static bool validateMenuChoice(const MyString& choice, int min, int max);
    static bool caseInsensitiveEqual(const MyString& a, const MyString& b);
};

#endif
