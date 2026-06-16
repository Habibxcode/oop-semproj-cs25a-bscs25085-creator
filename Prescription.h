#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include "MyString.h"

class Prescription {
private:
    int prescriptionId;
    int appointmentId;
    int patientId;
    int doctorId;
    MyString date;
    MyString medicines;
    MyString notes;

public:
    Prescription();
    Prescription(int prescriptionId, int appointmentId, int patientId, int doctorId, const MyString& date, const MyString& medicines, const MyString& notes);

    int getPrescriptionId() const;
    int getAppointmentId() const;
    int getPatientId() const;
    int getDoctorId() const;
    MyString getDate() const;
    MyString getMedicines() const;
    MyString getNotes() const;

    void setMedicines(const MyString& newMedicines);
    void setNotes(const MyString& newNotes);
};

#endif
