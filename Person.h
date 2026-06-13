#ifndef PERSON_H
#define PERSON_H

#include "MyString.h"

class Person {
protected:
    int id;
    MyString name;
    MyString contact;
    MyString password;

public:
    Person();
    Person(int id, const MyString& name, const MyString& contact, const MyString& password);
    virtual ~Person();

    virtual void displayInfo() = 0;
    virtual MyString getRole() = 0;

    int getId() const;
    MyString getName() const;
    MyString getContact() const;
    MyString getPassword() const;

    void setId(int newId);
    void setName(const MyString& newName);
    void setContact(const MyString& newContact);
    void setPassword(const MyString& newPassword);
};

#endif
