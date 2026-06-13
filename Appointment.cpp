#include "Appointment.h"

Appointment::Appointment() {
    appointmentId = 0;
    patientId = 0;
    doctorId = 0;
}

Appointment::Appointment(int appointmentId, int patientId, int doctorId, const MyString& date, const MyString& timeSlot, const MyString& status) {
    this->appointmentId = appointmentId;
    this->patientId = patientId;
    this->doctorId = doctorId;
    this->date = date;
    this->timeSlot = timeSlot;
    this->status = status;
}

int Appointment::getAppointmentId() const {
    return appointmentId;
}

int Appointment::getPatientId() const {
    return patientId;
}

int Appointment::getDoctorId() const {
    return doctorId;
}

MyString Appointment::getDate() const {
    return date;
}

MyString Appointment::getTimeSlot() const {
    return timeSlot;
}

MyString Appointment::getStatus() const {
    return status;
}

void Appointment::setStatus(const MyString& newStatus) {
    status = newStatus;
}

bool Appointment::operator==(const Appointment& other) const {
    if (doctorId != other.doctorId) return false;
    if (date != other.date) return false;
    if (timeSlot != other.timeSlot) return false;
    if (status == "cancelled" || other.status == "cancelled") return false;
    return true;
}

std::ostream& operator<<(std::ostream& os, const Appointment& appt) {
    os << "ID: " << appt.appointmentId << " | Patient: " << appt.patientId
       << " | Doctor: " << appt.doctorId << " | Date: " << appt.date
       << " | Time: " << appt.timeSlot << " | Status: " << appt.status;
    return os;
}
