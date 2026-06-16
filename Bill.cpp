#include "Bill.h"

Bill::Bill() {
    billId = 0;
    patientId = 0;
    appointmentId = 0;
    amount = 0.0f;
}

Bill::Bill(int billId, int patientId, int appointmentId, float amount, const MyString& status, const MyString& date) {
    this->billId = billId;
    this->patientId = patientId;
    this->appointmentId = appointmentId;
    this->amount = amount;
    this->status = status;
    this->date = date;
}

int Bill::getBillId() const {
    return billId;
}

int Bill::getPatientId() const {
    return patientId;
}

int Bill::getAppointmentId() const {
    return appointmentId;
}

float Bill::getAmount() const {
    return amount;
}

MyString Bill::getStatus() const {
    return status;
}

MyString Bill::getDate() const {
    return date;
}

void Bill::setStatus(const MyString& newStatus) {
    status = newStatus;
}

void Bill::setAmount(float newAmount) {
    amount = newAmount;
}
