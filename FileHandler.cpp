#define _CRT_SECURE_NO_WARNINGS
#include "FileHandler.h"
#include "FileNotFoundException.h"
#include <cstdio>

void FileHandler::stripNewline(char* line) {
    int i = 0;
    while (line[i] != '\0') {
        if (line[i] == '\n' || line[i] == '\r') {
            line[i] = '\0';
            break;
        }
        i++;
    }
}

void FileHandler::splitLine(const char* line, MyVector<MyString>& fields) {
    fields.clear();
    MyString current;
    int i = 0;
    while (line[i] != '\0') {
        if (line[i] == ',') {
            fields.add(current);
            current.clear();
        } else {
            char temp[2];
            temp[0] = line[i];
            temp[1] = '\0';
            current += temp;
        }
        i++;
    }
    fields.add(current);
}

void FileHandler::rewriteFile(const char* filename, MyVector<MyString>& lines) {
    FILE* file = fopen(filename, "w");
    if (file == nullptr) {
        throw FileNotFoundException("Could not open file for writing");
    }
    for (int i = 0; i < lines.size(); i++) {
        fprintf(file, "%s\n", lines[i].c_str());
    }
    fclose(file);
}

void FileHandler::loadPatients(MyVector<Patient>& patients) {
    FILE* file = fopen("patients.txt", "r");
    if (file == nullptr) {
        throw FileNotFoundException("Could not open patients.txt");
    }
    char line[1024];
    fgets(line, 1024, file);
    MyVector<MyString> fields;
    while (fgets(line, 1024, file) != nullptr) {
        stripNewline(line);
        if (line[0] == '\0') continue;
        splitLine(line, fields);
        if (fields.size() < 7) continue;
        int id = fields[0].toInt();
        MyString name = fields[1];
        int age = fields[2].toInt();
        MyString gender = fields[3];
        MyString contact = fields[4];
        MyString password = fields[5];
        float balance = fields[6].toFloat();
        patients.add(Patient(id, name, age, gender, contact, password, balance));
    }
    fclose(file);
}

void FileHandler::loadDoctors(MyVector<Doctor>& doctors) {
    FILE* file = fopen("doctors.txt", "r");
    if (file == nullptr) {
        throw FileNotFoundException("Could not open doctors.txt");
    }
    char line[1024];
    fgets(line, 1024, file);
    MyVector<MyString> fields;
    while (fgets(line, 1024, file) != nullptr) {
        stripNewline(line);
        if (line[0] == '\0') continue;
        splitLine(line, fields);
        if (fields.size() < 6) continue;
        int id = fields[0].toInt();
        MyString name = fields[1];
        MyString specialization = fields[2];
        MyString contact = fields[3];
        MyString password = fields[4];
        float fee = fields[5].toFloat();
        doctors.add(Doctor(id, name, specialization, contact, password, fee));
    }
    fclose(file);
}

void FileHandler::loadAdmin(Admin& admin) {
    FILE* file = fopen("admin.txt", "r");
    if (file == nullptr) {
        throw FileNotFoundException("Could not open admin.txt");
    }
    char line[1024];
    fgets(line, 1024, file);
    MyVector<MyString> fields;
    if (fgets(line, 1024, file) != nullptr) {
        stripNewline(line);
        splitLine(line, fields);
        if (fields.size() >= 3) {
            admin.setId(fields[0].toInt());
            admin.setName(fields[1]);
            admin.setPassword(fields[2]);
        }
    }
    fclose(file);
}

void FileHandler::loadAppointments(MyVector<Appointment>& appointments) {
    FILE* file = fopen("appointments.txt", "r");
    if (file == nullptr) {
        throw FileNotFoundException("Could not open appointments.txt");
    }
    char line[1024];
    fgets(line, 1024, file);
    MyVector<MyString> fields;
    while (fgets(line, 1024, file) != nullptr) {
        stripNewline(line);
        if (line[0] == '\0') continue;
        splitLine(line, fields);
        if (fields.size() < 6) continue;
        int appointmentId = fields[0].toInt();
        int patientId = fields[1].toInt();
        int doctorId = fields[2].toInt();
        MyString date = fields[3];
        MyString timeSlot = fields[4];
        MyString status = fields[5];
        appointments.add(Appointment(appointmentId, patientId, doctorId, date, timeSlot, status));
    }
    fclose(file);
}

void FileHandler::loadPrescriptions(MyVector<Prescription>& prescriptions) {
    FILE* file = fopen("prescriptions.txt", "r");
    if (file == nullptr) {
        throw FileNotFoundException("Could not open prescriptions.txt");
    }
    char line[1024];
    fgets(line, 1024, file);
    MyVector<MyString> fields;
    while (fgets(line, 1024, file) != nullptr) {
        stripNewline(line);
        if (line[0] == '\0') continue;
        splitLine(line, fields);
        if (fields.size() < 7) continue;
        int prescriptionId = fields[0].toInt();
        int appointmentId = fields[1].toInt();
        int patientId = fields[2].toInt();
        int doctorId = fields[3].toInt();
        MyString date = fields[4];
        MyString medicines = fields[5];
        MyString notes = fields[6];
        prescriptions.add(Prescription(prescriptionId, appointmentId, patientId, doctorId, date, medicines, notes));
    }
    fclose(file);
}

void FileHandler::loadBills(MyVector<Bill>& bills) {
    FILE* file = fopen("bills.txt", "r");
    if (file == nullptr) {
        throw FileNotFoundException("Could not open bills.txt");
    }
    char line[1024];
    fgets(line, 1024, file);
    MyVector<MyString> fields;
    while (fgets(line, 1024, file) != nullptr) {
        stripNewline(line);
        if (line[0] == '\0') continue;
        splitLine(line, fields);
        if (fields.size() < 6) continue;
        int billId = fields[0].toInt();
        int patientId = fields[1].toInt();
        int appointmentId = fields[2].toInt();
        float amount = fields[3].toFloat();
        MyString status = fields[4];
        MyString date = fields[5];
        bills.add(Bill(billId, patientId, appointmentId, amount, status, date));
    }
    fclose(file);
}

void FileHandler::appendLine(const char* filename, const char* line) {
    FILE* file = fopen(filename, "a");
    if (file == nullptr) {
        throw FileNotFoundException("Could not open file for appending");
    }
    fprintf(file, "%s\n", line);
    fclose(file);
}

void FileHandler::updateLine(const char* filename, int id, const char* newLine) {
    MyVector<MyString> lines;
    readAllLines(filename, lines);
    MyVector<MyString> fields;
    for (int i = 1; i < lines.size(); i++) {
        splitLine(lines[i].c_str(), fields);
        if (fields.size() > 0 && fields[0].toInt() == id) {
            lines[i] = MyString(newLine);
        }
    }
    rewriteFile(filename, lines);
}

void FileHandler::deleteLine(const char* filename, int id) {
    MyVector<MyString> lines;
    readAllLines(filename, lines);
    MyVector<MyString> result;
    MyVector<MyString> fields;
    if (lines.size() > 0) {
        result.add(lines[0]);
    }
    for (int i = 1; i < lines.size(); i++) {
        splitLine(lines[i].c_str(), fields);
        if (fields.size() > 0 && fields[0].toInt() == id) {
            continue;
        }
        result.add(lines[i]);
    }
    rewriteFile(filename, result);
}

void FileHandler::readAllLines(const char* filename, MyVector<MyString>& lines) {
    lines.clear();
    FILE* file = fopen(filename, "r");
    if (file == nullptr) {
        throw FileNotFoundException("Could not open file for reading");
    }
    char line[1024];
    while (fgets(line, 1024, file) != nullptr) {
        stripNewline(line);
        lines.add(MyString(line));
    }
    fclose(file);
}

int FileHandler::generateNextId(const char* filename) {
    MyVector<MyString> lines;
    readAllLines(filename, lines);
    MyVector<MyString> fields;
    int maxId = 0;
    for (int i = 1; i < lines.size(); i++) {
        if (lines[i].isEmpty()) continue;
        splitLine(lines[i].c_str(), fields);
        if (fields.size() > 0) {
            int currentId = fields[0].toInt();
            if (currentId > maxId) maxId = currentId;
        }
    }
    return maxId + 1;
}
