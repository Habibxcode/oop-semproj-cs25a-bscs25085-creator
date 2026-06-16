#ifndef DOCTOR_H
#define DOCTOR_H

#include "Person.h"
#include <iostream>

class Doctor : public Person {
private:
    MyString specialization;
    float fee;

public:
    Doctor();
    Doctor(int id, const MyString& name, const MyString& specialization, const MyString& contact, const MyString& password, float fee);

    void displayInfo();
    MyString getRole();

    MyString getSpecialization() const;
    float getFee() const;

    void setSpecialization(const MyString& newSpecialization);
    void setFee(float newFee);

    bool operator==(const Doctor& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Doctor& doctor);
};

#endif
