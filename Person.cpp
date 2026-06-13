#include "Person.h"

Person::Person() {
    id = 0;
}

Person::Person(int id, const MyString& name, const MyString& contact, const MyString& password) {
    this->id = id;
    this->name = name;
    this->contact = contact;
    this->password = password;
}

Person::~Person() {
}

int Person::getId() const {
    return id;
}

MyString Person::getName() const {
    return name;
}

MyString Person::getContact() const {
    return contact;
}

MyString Person::getPassword() const {
    return password;
}

void Person::setId(int newId) {
    id = newId;
}

void Person::setName(const MyString& newName) {
    name = newName;
}

void Person::setContact(const MyString& newContact) {
    contact = newContact;
}

void Person::setPassword(const MyString& newPassword) {
    password = newPassword;
}
