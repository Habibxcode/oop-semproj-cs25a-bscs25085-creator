#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"
#include <iostream>

class Patient : public Person {
private:
    int age;
    MyString gender;
    float balance;

public:
    Patient();
    Patient(int id, const MyString& name, int age, const MyString& gender, const MyString& contact, const MyString& password, float balance);

    void displayInfo();
    MyString getRole();

    int getAge() const;
    MyString getGender() const;
    float getBalance() const;

    void setAge(int newAge);
    void setGender(const MyString& newGender);
    void setBalance(float newBalance);

    Patient& operator+=(float amount);
    Patient& operator-=(float amount);
    bool operator==(const Patient& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Patient& patient);
};

#endif
