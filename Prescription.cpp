#include "Prescription.h"

Prescription::Prescription() {
    prescriptionId = 0;
    appointmentId = 0;
    patientId = 0;
    doctorId = 0;
}

Prescription::Prescription(int prescriptionId, int appointmentId, int patientId, int doctorId, const MyString& date, const MyString& medicines, const MyString& notes) {
    this->prescriptionId = prescriptionId;
    this->appointmentId = appointmentId;
    this->patientId = patientId;
    this->doctorId = doctorId;
    this->date = date;
    setMedicines(medicines);
    setNotes(notes);
}

int Prescription::getPrescriptionId() const {
    return prescriptionId;
}

int Prescription::getAppointmentId() const {
    return appointmentId;
}

int Prescription::getPatientId() const {
    return patientId;
}

int Prescription::getDoctorId() const {
    return doctorId;
}

MyString Prescription::getDate() const {
    return date;
}

MyString Prescription::getMedicines() const {
    return medicines;
}

MyString Prescription::getNotes() const {
    return notes;
}

void Prescription::setMedicines(const MyString& newMedicines) {
    if (newMedicines.length() > 500) {
        medicines = newMedicines.substr(0, 500);
    } else {
        medicines = newMedicines;
    }
}

void Prescription::setNotes(const MyString& newNotes) {
    if (newNotes.length() > 300) {
        notes = newNotes.substr(0, 300);
    } else {
        notes = newNotes;
    }
}
