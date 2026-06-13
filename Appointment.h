#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include "MyString.h"
#include <iostream>

class Appointment {
private:
    int appointmentId;
    int patientId;
    int doctorId;
    MyString date;
    MyString timeSlot;
    MyString status;

public:
    Appointment();
    Appointment(int appointmentId, int patientId, int doctorId, const MyString& date, const MyString& timeSlot, const MyString& status);

    int getAppointmentId() const;
    int getPatientId() const;
    int getDoctorId() const;
    MyString getDate() const;
    MyString getTimeSlot() const;
    MyString getStatus() const;

    void setStatus(const MyString& newStatus);

    bool operator==(const Appointment& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Appointment& appt);
};

#endif
