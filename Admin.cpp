#include "Admin.h"
#include <iostream>

Admin::Admin() : Person() {
}

Admin::Admin(int id, const MyString& name, const MyString& password)
    : Person(id, name, MyString(""), password) {
}

void Admin::displayInfo() {
    std::cout << "ID: " << id << " Name: " << name << std::endl;
}

MyString Admin::getRole() {
    return MyString("admin");
}
