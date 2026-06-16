#include "Doctor.h"

Doctor::Doctor() : Person() {
    fee = 0.0f;
}

Doctor::Doctor(int id, const MyString& name, const MyString& specialization, const MyString& contact, const MyString& password, float fee)
    : Person(id, name, contact, password) {
    this->specialization = specialization;
    this->fee = fee;
}

void Doctor::displayInfo() {
    std::cout << "ID: " << id << " Name: " << name << " Specialization: " << specialization
              << " Contact: " << contact << " Fee: " << fee << std::endl;
}

MyString Doctor::getRole() {
    return MyString("doctor");
}

MyString Doctor::getSpecialization() const {
    return specialization;
}

float Doctor::getFee() const {
    return fee;
}

void Doctor::setSpecialization(const MyString& newSpecialization) {
    specialization = newSpecialization;
}

void Doctor::setFee(float newFee) {
    fee = newFee;
}

bool Doctor::operator==(const Doctor& other) const {
    return id == other.id;
}

std::ostream& operator<<(std::ostream& os, const Doctor& doctor) {
    os << "ID: " << doctor.id << " | Name: " << doctor.name
       << " | Specialization: " << doctor.specialization
       << " | Contact: " << doctor.contact << " | Fee: " << doctor.fee;
    return os;
}
