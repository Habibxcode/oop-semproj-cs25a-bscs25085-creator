#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"

class Admin : public Person {
public:
    Admin();
    Admin(int id, const MyString& name, const MyString& password);

    void displayInfo();
    MyString getRole();
};

#endif
