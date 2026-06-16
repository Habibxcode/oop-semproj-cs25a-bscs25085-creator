#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "MyVector.h"
#include "MyString.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"

class FileHandler {
private:
    void splitLine(const char* line, MyVector<MyString>& fields);
    void stripNewline(char* line);
    void rewriteFile(const char* filename, MyVector<MyString>& lines);

public:
    void loadPatients(MyVector<Patient>& patients);
    void loadDoctors(MyVector<Doctor>& doctors);
    void loadAdmin(Admin& admin);
    void loadAppointments(MyVector<Appointment>& appointments);
    void loadPrescriptions(MyVector<Prescription>& prescriptions);
    void loadBills(MyVector<Bill>& bills);

    void appendLine(const char* filename, const char* line);
    void updateLine(const char* filename, int id, const char* newLine);
    void deleteLine(const char* filename, int id);
    void readAllLines(const char* filename, MyVector<MyString>& lines);

    int generateNextId(const char* filename);
};

#endif
