#include "Patient.h"

Patient::Patient() : Person() {
    age = 0;
    balance = 0.0f;
}

Patient::Patient(int id, const MyString& name, int age, const MyString& gender, const MyString& contact, const MyString& password, float balance)
    : Person(id, name, contact, password) {
    this->age = age;
    this->gender = gender;
    this->balance = balance;
}

void Patient::displayInfo() {
    std::cout << "ID: " << id << " Name: " << name << " Age: " << age
              << " Gender: " << gender << " Contact: " << contact
              << " Balance: " << balance << std::endl;
}

MyString Patient::getRole() {
    return MyString("patient");
}

int Patient::getAge() const {
    return age;
}

MyString Patient::getGender() const {
    return gender;
}

float Patient::getBalance() const {
    return balance;
}

void Patient::setAge(int newAge) {
    age = newAge;
}

void Patient::setGender(const MyString& newGender) {
    gender = newGender;
}

void Patient::setBalance(float newBalance) {
    balance = newBalance;
}

Patient& Patient::operator+=(float amount) {
    balance += amount;
    return *this;
}

Patient& Patient::operator-=(float amount) {
    balance -= amount;
    return *this;
}

bool Patient::operator==(const Patient& other) const {
    return id == other.id;
}

std::ostream& operator<<(std::ostream& os, const Patient& patient) {
    os << "ID: " << patient.id << " | Name: " << patient.name
       << " | Age: " << patient.age << " | Gender: " << patient.gender
       << " | Contact: " << patient.contact << " | Balance: " << patient.balance;
    return os;
}
