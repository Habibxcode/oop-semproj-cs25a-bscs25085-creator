#ifndef BILL_H
#define BILL_H

#include "MyString.h"

class Bill {
private:
    int billId;
    int patientId;
    int appointmentId;
    float amount;
    MyString status;
    MyString date;

public:
    Bill();
    Bill(int billId, int patientId, int appointmentId, float amount, const MyString& status, const MyString& date);

    int getBillId() const;
    int getPatientId() const;
    int getAppointmentId() const;
    float getAmount() const;
    MyString getStatus() const;
    MyString getDate() const;

    void setStatus(const MyString& newStatus);
    void setAmount(float newAmount);
};

#endif
