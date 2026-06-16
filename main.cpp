#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <raylib.h>
#include "MyString.h"
#include "MyVector.h"
#include "Person.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "FileHandler.h"
#include "Validator.h"
#include "HospitalException.h"
#include "FileNotFoundException.h"
#include "InsufficientFundsException.h"
#include "InvalidInputException.h"
#include "SlotUnavailableException.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800
#define FONT_SIZE_TITLE 36
#define FONT_SIZE_HEADING 28
#define FONT_SIZE_BODY 20

enum AppScreen {
    SCREEN_ROLE_SELECT,
    SCREEN_LOGIN,
    SCREEN_PATIENT_MENU,
    SCREEN_PATIENT_BOOK,
    SCREEN_PATIENT_CANCEL,
    SCREEN_PATIENT_VIEW_APPTS,
    SCREEN_PATIENT_VIEW_RECORDS,
    SCREEN_PATIENT_VIEW_BILLS,
    SCREEN_PATIENT_PAY_BILL,
    SCREEN_PATIENT_TOPUP,
    SCREEN_DOCTOR_MENU,
    SCREEN_DOCTOR_TODAY,
    SCREEN_DOCTOR_COMPLETE,
    SCREEN_DOCTOR_NOSHOW,
    SCREEN_DOCTOR_PRESCRIBE,
    SCREEN_DOCTOR_HISTORY,
    SCREEN_ADMIN_MENU,
    SCREEN_ADMIN_ADD_DOCTOR,
    SCREEN_ADMIN_REMOVE_DOCTOR,
    SCREEN_ADMIN_ALL_PATIENTS,
    SCREEN_ADMIN_ALL_DOCTORS,
    SCREEN_ADMIN_ALL_APPTS,
    SCREEN_ADMIN_UNPAID_BILLS,
    SCREEN_ADMIN_DISCHARGE,
    SCREEN_ADMIN_SECURITY_LOG,
    SCREEN_ADMIN_DAILY_REPORT,
    SCREEN_LOCKED
};

enum AppRole {
    ROLE_NONE,
    ROLE_PATIENT,
    ROLE_DOCTOR,
    ROLE_ADMIN
};

Color colorBackground() { return Color{ 13, 27, 42, 255 }; }
Color colorPanel() { return Color{ 27, 42, 59, 255 }; }
Color colorPanelAlt() { return Color{ 22, 33, 48, 255 }; }
Color colorAccent() { return Color{ 0, 180, 216, 255 }; }
Color colorAccentHover() { return Color{ 72, 202, 228, 255 }; }
Color colorTextPrimary() { return Color{ 255, 255, 255, 255 }; }
Color colorTextSecondary() { return Color{ 160, 174, 192, 255 }; }
Color colorErrorText() { return Color{ 255, 107, 107, 255 }; }
Color colorSuccessText() { return Color{ 81, 207, 102, 255 }; }

class AppState {
public:
    FileHandler fileHandler;
    MyVector<Patient> patients;
    MyVector<Doctor> doctors;
    MyVector<Appointment> appointments;
    MyVector<Bill> bills;
    MyVector<Prescription> prescriptions;
    Admin admin;

    int currentScreen;
    int selectedRole;
    int cursorTimer;

    MyString loginIdInput;
    MyString loginPasswordInput;
    bool loginIdActive;
    bool loginPasswordActive;
    int loginAttempts;
    MyString loginError;

    int currentPatientIndex;
    int currentDoctorIndex;

    MyString successMessage;
    int successTimer;

    MyString flashMessage;
    int flashTimer;
    bool flashIsError;

    int scrollOffset;

    int bookStep;
    MyString bookSpecializationInput;
    bool bookSpecializationActive;
    MyString bookDoctorIdInput;
    bool bookDoctorIdActive;
    MyString bookDateInput;
    bool bookDateActive;
    int bookDateAttempts;
    MyString bookError;

    MyString cancelIdInput;
    bool cancelIdActive;
    MyString cancelError;

    MyString payBillIdInput;
    bool payBillIdActive;
    MyString payBillError;

    MyString topupAmountInput;
    bool topupAmountActive;
    MyString topupError;
    int topupAttempts;

    MyString doctorApptIdInput;
    bool doctorApptIdActive;
    MyString doctorError;

    MyString prescribeMedicinesInput;
    bool prescribeMedicinesActive;
    MyString prescribeNotesInput;
    bool prescribeNotesActive;

    MyString historyPatientIdInput;
    bool historyPatientIdActive;
    MyString historyError;

    MyString addDocNameInput;
    bool addDocNameActive;
    MyString addDocSpecInput;
    bool addDocSpecActive;
    MyString addDocContactInput;
    bool addDocContactActive;
    MyString addDocPasswordInput;
    bool addDocPasswordActive;
    MyString addDocFeeInput;
    bool addDocFeeActive;
    MyString addDocError;

    MyString removeDocIdInput;
    bool removeDocIdActive;
    MyString removeDocError;

    MyString dischargeIdInput;
    bool dischargeIdActive;
    MyString dischargeError;

    AppState() {
        currentScreen = SCREEN_ROLE_SELECT;
        selectedRole = ROLE_NONE;
        cursorTimer = 0;
        loginIdActive = false;
        loginPasswordActive = false;
        loginAttempts = 0;
        currentPatientIndex = -1;
        currentDoctorIndex = -1;
        successTimer = 0;
        flashTimer = 0;
        flashIsError = false;
        scrollOffset = 0;
        bookStep = 0;
        bookSpecializationActive = false;
        bookDoctorIdActive = false;
        bookDateActive = false;
        bookDateAttempts = 0;
        cancelIdActive = false;
        payBillIdActive = false;
        topupAmountActive = false;
        topupAttempts = 0;
        doctorApptIdActive = false;
        prescribeMedicinesActive = false;
        prescribeNotesActive = false;
        historyPatientIdActive = false;
        addDocNameActive = false;
        addDocSpecActive = false;
        addDocContactActive = false;
        addDocPasswordActive = false;
        addDocFeeActive = false;
        removeDocIdActive = false;
        dischargeIdActive = false;
    }
};

MyString getCurrentDateString() {
    time_t now = time(0);
    tm* lt = localtime(&now);
    char buffer[16];
    sprintf(buffer, "%02d-%02d-%04d", lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
    return MyString(buffer);
}

MyString getCurrentTimestampString() {
    time_t now = time(0);
    tm* lt = localtime(&now);
    char buffer[32];
    sprintf(buffer, "%02d-%02d-%04d %02d:%02d:%02d", lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900, lt->tm_hour, lt->tm_min, lt->tm_sec);
    return MyString(buffer);
}

int dateToInt(const MyString& date) {
    if (date.length() != 10) return 0;
    int day = date.substr(0, 2).toInt();
    int month = date.substr(3, 2).toInt();
    int year = date.substr(6, 4).toInt();
    return year * 10000 + month * 100 + day;
}

MyString timeSlotByIndex(int index) {
    if (index == 0) return MyString("09:00");
    if (index == 1) return MyString("10:00");
    if (index == 2) return MyString("11:00");
    if (index == 3) return MyString("12:00");
    if (index == 4) return MyString("13:00");
    if (index == 5) return MyString("14:00");
    if (index == 6) return MyString("15:00");
    return MyString("16:00");
}

bool myStringLess(const MyString& a, const MyString& b) {
    int i = 0;
    while (i < a.length() && i < b.length()) {
        if (a.charAt(i) != b.charAt(i)) return a.charAt(i) < b.charAt(i);
        i++;
    }
    return a.length() < b.length();
}

MyString intToMyString(int value) {
    char buffer[16];
    sprintf(buffer, "%d", value);
    return MyString(buffer);
}

MyString floatToMyString(float value) {
    char buffer[32];
    sprintf(buffer, "%.2f", value);
    return MyString(buffer);
}

MyString buildPatientLine(const Patient& p) {
    MyString line = intToMyString(p.getId());
    line += ",";
    line += p.getName();
    line += ",";
    line += intToMyString(p.getAge());
    line += ",";
    line += p.getGender();
    line += ",";
    line += p.getContact();
    line += ",";
    line += p.getPassword();
    line += ",";
    line += floatToMyString(p.getBalance());
    return line;
}

MyString buildAppointmentLine(const Appointment& a) {
    MyString line = intToMyString(a.getAppointmentId());
    line += ",";
    line += intToMyString(a.getPatientId());
    line += ",";
    line += intToMyString(a.getDoctorId());
    line += ",";
    line += a.getDate();
    line += ",";
    line += a.getTimeSlot();
    line += ",";
    line += a.getStatus();
    return line;
}

MyString buildBillLine(const Bill& b) {
    MyString line = intToMyString(b.getBillId());
    line += ",";
    line += intToMyString(b.getPatientId());
    line += ",";
    line += intToMyString(b.getAppointmentId());
    line += ",";
    line += floatToMyString(b.getAmount());
    line += ",";
    line += b.getStatus();
    line += ",";
    line += b.getDate();
    return line;
}

MyString buildPrescriptionLine(const Prescription& p) {
    MyString line = intToMyString(p.getPrescriptionId());
    line += ",";
    line += intToMyString(p.getAppointmentId());
    line += ",";
    line += intToMyString(p.getPatientId());
    line += ",";
    line += intToMyString(p.getDoctorId());
    line += ",";
    line += p.getDate();
    line += ",";
    line += p.getMedicines();
    line += ",";
    line += p.getNotes();
    return line;
}

MyString buildDoctorLine(const Doctor& d) {
    MyString line = intToMyString(d.getId());
    line += ",";
    line += d.getName();
    line += ",";
    line += d.getSpecialization();
    line += ",";
    line += d.getContact();
    line += ",";
    line += d.getPassword();
    line += ",";
    line += floatToMyString(d.getFee());
    return line;
}

int findPatientIndexById(AppState* state, int id) {
    for (int i = 0; i < state->patients.size(); i++) {
        if (state->patients[i].getId() == id) return i;
    }
    return -1;
}

int findDoctorIndexById(AppState* state, int id) {
    for (int i = 0; i < state->doctors.size(); i++) {
        if (state->doctors[i].getId() == id) return i;
    }
    return -1;
}

MyString getDoctorName(AppState* state, int doctorId) {
    int idx = findDoctorIndexById(state, doctorId);
    if (idx == -1) return MyString("Unknown");
    return state->doctors[idx].getName();
}

MyString getDoctorSpecialization(AppState* state, int doctorId) {
    int idx = findDoctorIndexById(state, doctorId);
    if (idx == -1) return MyString("Unknown");
    return state->doctors[idx].getSpecialization();
}

MyString getPatientName(AppState* state, int patientId) {
    int idx = findPatientIndexById(state, patientId);
    if (idx == -1) return MyString("Unknown");
    return state->patients[idx].getName();
}

void sortAppointmentsByDateAsc(MyVector<Appointment>* list) {
    for (int i = 0; i < list->size() - 1; i++) {
        for (int j = 0; j < list->size() - 1 - i; j++) {
            if (dateToInt((*list)[j].getDate()) > dateToInt((*list)[j + 1].getDate())) {
                Appointment temp = (*list)[j];
                (*list)[j] = (*list)[j + 1];
                (*list)[j + 1] = temp;
            }
        }
    }
}

void sortAppointmentsByDateDesc(MyVector<Appointment>* list) {
    for (int i = 0; i < list->size() - 1; i++) {
        for (int j = 0; j < list->size() - 1 - i; j++) {
            if (dateToInt((*list)[j].getDate()) < dateToInt((*list)[j + 1].getDate())) {
                Appointment temp = (*list)[j];
                (*list)[j] = (*list)[j + 1];
                (*list)[j + 1] = temp;
            }
        }
    }
}

void sortPrescriptionsByDateDesc(MyVector<Prescription>* list) {
    for (int i = 0; i < list->size() - 1; i++) {
        for (int j = 0; j < list->size() - 1 - i; j++) {
            if (dateToInt((*list)[j].getDate()) < dateToInt((*list)[j + 1].getDate())) {
                Prescription temp = (*list)[j];
                (*list)[j] = (*list)[j + 1];
                (*list)[j + 1] = temp;
            }
        }
    }
}

void sortAppointmentsByTimeSlotAsc(MyVector<Appointment>* list) {
    for (int i = 0; i < list->size() - 1; i++) {
        for (int j = 0; j < list->size() - 1 - i; j++) {
            if (myStringLess((*list)[j + 1].getTimeSlot(), (*list)[j].getTimeSlot())) {
                Appointment temp = (*list)[j];
                (*list)[j] = (*list)[j + 1];
                (*list)[j + 1] = temp;
            }
        }
    }
}

bool drawButton(Rectangle rect, const char* label) {
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rect);
    Color fillColor = hover ? colorAccentHover() : colorAccent();
    DrawRectangleRounded(rect, 0.3f, 8, fillColor);
    int textWidth = MeasureText(label, FONT_SIZE_BODY);
    int textX = (int)(rect.x + (rect.width - textWidth) / 2);
    int textY = (int)(rect.y + (rect.height - FONT_SIZE_BODY) / 2);
    DrawText(label, textX, textY, FONT_SIZE_BODY, colorTextPrimary());
    return hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void drawPanel(Rectangle rect) {
    DrawRectangleRounded(rect, 0.04f, 8, colorPanel());
}

void drawBackButton(AppState* state, int targetScreen) {
    Rectangle rect = Rectangle{ 20, 20, 100, 40 };
    if (drawButton(rect, "Back")) {
        state->currentScreen = targetScreen;
        state->scrollOffset = 0;
    }
}

void drawCenteredText(const char* text, int y, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, (WINDOW_WIDTH - textWidth) / 2, y, fontSize, color);
}

void showSuccessMessage(AppState* state, const char* msg) {
    state->successMessage = msg;
    state->successTimer = 120;
}

void updateSuccessTimer(AppState* state) {
    if (state->successTimer > 0) {
        state->successTimer--;
        if (state->successTimer == 0) state->successMessage.clear();
    }
}

void drawSuccessMessage(AppState* state, int x, int y) {
    if (state->successTimer > 0) {
        DrawText(state->successMessage.c_str(), x, y, FONT_SIZE_BODY, colorSuccessText());
    }
}

void drawErrorText(const MyString& msg, int x, int y) {
    if (!msg.isEmpty()) {
        DrawText(msg.c_str(), x, y, FONT_SIZE_BODY, colorErrorText());
    }
}

void setFlashMessage(AppState* state, const char* msg, bool isError) {
    state->flashMessage = msg;
    state->flashIsError = isError;
    state->flashTimer = 180;
}

void updateFlashTimer(AppState* state) {
    if (state->flashTimer > 0) {
        state->flashTimer--;
        if (state->flashTimer == 0) state->flashMessage.clear();
    }
}

void drawFlashMessage(AppState* state, int x, int y) {
    if (state->flashTimer > 0) {
        Color color = state->flashIsError ? colorErrorText() : colorSuccessText();
        DrawText(state->flashMessage.c_str(), x, y, FONT_SIZE_BODY, color);
    }
}

void updateTextField(Rectangle rect, MyString* value, bool* active, int maxLen) {
    Vector2 mouse = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *active = CheckCollisionPointRec(mouse, rect);
    }
    if (*active) {
        int key = GetCharPressed();
        while (key > 0) {
            if (value->length() < maxLen && key >= 32 && key <= 125) {
                char temp[2];
                temp[0] = (char)key;
                temp[1] = '\0';
                (*value) += temp;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
            if (value->length() > 0) {
                *value = value->substr(0, value->length() - 1);
            }
        }
    }
}

void drawTextField(Rectangle rect, const MyString& value, bool active, bool masked, int cursorTimer) {
    DrawRectangleRounded(rect, 0.2f, 8, colorPanelAlt());
    Color borderColor = active ? colorAccent() : colorPanel();
    DrawRectangleRoundedLines(rect, 0.2f, 8, borderColor); 
    MyString display;
    if (masked) {
        for (int i = 0; i < value.length(); i++) display += "*";
    } else {
        display = value;
    }
    DrawText(display.c_str(), (int)rect.x + 10, (int)rect.y + (int)(rect.height / 2) - FONT_SIZE_BODY / 2, FONT_SIZE_BODY, colorTextPrimary());
    if (active && (cursorTimer / 30) % 2 == 0) {
        int textWidth = MeasureText(display.c_str(), FONT_SIZE_BODY);
        int cursorX = (int)rect.x + 10 + textWidth + 2;
        DrawLine(cursorX, (int)rect.y + 8, cursorX, (int)rect.y + (int)rect.height - 8, colorTextPrimary());
    }
}

void drawFieldLabel(const char* text, int x, int y) {
    DrawText(text, x, y, FONT_SIZE_BODY, colorTextSecondary());
}

void drawTableHeader(int x, int y, int* colWidths, MyString* headers, int colCount) {
    int curX = x;
    for (int i = 0; i < colCount; i++) {
        DrawRectangle(curX, y, colWidths[i], 36, colorAccent());
        DrawText(headers[i].c_str(), curX + 8, y + 8, FONT_SIZE_BODY, colorTextPrimary());
        curX += colWidths[i];
    }
}

void drawTableRow(int x, int y, int* colWidths, MyString* cells, int colCount, bool alt) {
    Color bg = alt ? colorPanelAlt() : colorPanel();
    int totalWidth = 0;
    for (int i = 0; i < colCount; i++) totalWidth += colWidths[i];
    DrawRectangle(x, y, totalWidth, 36, bg);
    int curX = x;
    for (int i = 0; i < colCount; i++) {
        DrawText(cells[i].c_str(), curX + 8, y + 8, FONT_SIZE_BODY, colorTextPrimary());
        curX += colWidths[i];
    }
}

void updateScroll(AppState* state, int maxOffset) {
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        state->scrollOffset -= (int)(wheel * 20);
        if (state->scrollOffset < 0) state->scrollOffset = 0;
        if (state->scrollOffset > maxOffset) state->scrollOffset = maxOffset;
    }
    if (maxOffset <= 0) state->scrollOffset = 0;
}

int computeMaxScroll(int rowCount, int visibleHeight, int rowHeight) {
    int contentHeight = rowCount * rowHeight;
    int maxOffset = contentHeight - visibleHeight;
    if (maxOffset < 0) maxOffset = 0;
    return maxOffset;
}

void logSecurityEvent(AppState* state, const char* role, const MyString& enteredId, const char* result) {
    MyString line = getCurrentTimestampString();
    line += ",";
    line += role;
    line += ",";
    line += enteredId;
    line += ",";
    line += result;
    state->fileHandler.appendLine("security_log.txt", line.c_str());
}

void resetLoginFields(AppState* state) {
    state->loginIdInput.clear();
    state->loginPasswordInput.clear();
    state->loginIdActive = false;
    state->loginPasswordActive = false;
    state->loginError.clear();
}

const char* roleName(int role) {
    if (role == ROLE_PATIENT) return "Patient";
    if (role == ROLE_DOCTOR) return "Doctor";
    return "Admin";
}

void performLogin(AppState* state) {
    if (!Validator::validateId(state->loginIdInput)) {
        state->loginError = "Please enter a valid numeric ID.";
        return;
    }
    int enteredId = state->loginIdInput.toInt();
    bool success = false;
    if (state->selectedRole == ROLE_PATIENT) {
        int idx = findPatientIndexById(state, enteredId);
        if (idx != -1 && state->patients[idx].getPassword() == state->loginPasswordInput) {
            state->currentPatientIndex = idx;
            success = true;
        }
    } else if (state->selectedRole == ROLE_DOCTOR) {
        int idx = findDoctorIndexById(state, enteredId);
        if (idx != -1 && state->doctors[idx].getPassword() == state->loginPasswordInput) {
            state->currentDoctorIndex = idx;
            success = true;
        }
    } else if (state->selectedRole == ROLE_ADMIN) {
        if (state->admin.getId() == enteredId && state->admin.getPassword() == state->loginPasswordInput) {
            success = true;
        }
    }
    if (success) {
        logSecurityEvent(state, roleName(state->selectedRole), state->loginIdInput, "SUCCESS");
        state->loginAttempts = 0;
        if (state->selectedRole == ROLE_PATIENT) state->currentScreen = SCREEN_PATIENT_MENU;
        else if (state->selectedRole == ROLE_DOCTOR) state->currentScreen = SCREEN_DOCTOR_MENU;
        else state->currentScreen = SCREEN_ADMIN_MENU;
        resetLoginFields(state);
    } else {
        state->loginAttempts++;
        logSecurityEvent(state, roleName(state->selectedRole), state->loginIdInput, "FAILED");
        if (state->loginAttempts >= 3) {
            state->currentScreen = SCREEN_LOCKED;
        } else {
            state->loginError = "Invalid ID or password.";
        }
    }
}

void renderRoleSelect(AppState* state) {
    drawCenteredText("MediCore", 120, FONT_SIZE_TITLE, colorTextPrimary());
    drawCenteredText("Hospital Management System", 175, FONT_SIZE_HEADING, colorTextSecondary());

    int buttonWidth = 220;
    int buttonHeight = 70;
    int gap = 40;
    int totalWidth = buttonWidth * 3 + gap * 2;
    int startX = (WINDOW_WIDTH - totalWidth) / 2;
    int y = 400;

    Rectangle patientRect = Rectangle{ (float)startX, (float)y, (float)buttonWidth, (float)buttonHeight };
    Rectangle doctorRect = Rectangle{ (float)(startX + buttonWidth + gap), (float)y, (float)buttonWidth, (float)buttonHeight };
    Rectangle adminRect = Rectangle{ (float)(startX + 2 * (buttonWidth + gap)), (float)y, (float)buttonWidth, (float)buttonHeight };

    if (drawButton(patientRect, "Patient")) {
        state->selectedRole = ROLE_PATIENT;
        resetLoginFields(state);
        state->loginAttempts = 0;
        state->currentScreen = SCREEN_LOGIN;
    }
    if (drawButton(doctorRect, "Doctor")) {
        state->selectedRole = ROLE_DOCTOR;
        resetLoginFields(state);
        state->loginAttempts = 0;
        state->currentScreen = SCREEN_LOGIN;
    }
    if (drawButton(adminRect, "Admin")) {
        state->selectedRole = ROLE_ADMIN;
        resetLoginFields(state);
        state->loginAttempts = 0;
        state->currentScreen = SCREEN_LOGIN;
    }
}

void renderLogin(AppState* state) {
    drawBackButton(state, SCREEN_ROLE_SELECT);
    char title[32];
    sprintf(title, "%s Login", roleName(state->selectedRole));
    drawCenteredText(title, 100, FONT_SIZE_TITLE, colorTextPrimary());

    int panelWidth = 480;
    int panelX = (WINDOW_WIDTH - panelWidth) / 2;
    Rectangle panelRect = Rectangle{ (float)panelX, 200, (float)panelWidth, 320 };
    drawPanel(panelRect);

    Rectangle idRect = Rectangle{ (float)(panelX + 40), 260, (float)(panelWidth - 80), 50 };
    Rectangle passRect = Rectangle{ (float)(panelX + 40), 360, (float)(panelWidth - 80), 50 };

    drawFieldLabel("ID", panelX + 40, 235);
    drawFieldLabel("Password", panelX + 40, 335);

    updateTextField(idRect, &state->loginIdInput, &state->loginIdActive, 10);
    updateTextField(passRect, &state->loginPasswordInput, &state->loginPasswordActive, 30);

    drawTextField(idRect, state->loginIdInput, state->loginIdActive, false, state->cursorTimer);
    drawTextField(passRect, state->loginPasswordInput, state->loginPasswordActive, true, state->cursorTimer);

    drawErrorText(state->loginError, panelX + 40, 425);

    Rectangle loginBtn = Rectangle{ (float)(panelX + 40), 460, (float)(panelWidth - 80), 50 };
    if (drawButton(loginBtn, "Login")) {
        performLogin(state);
    }
}

void renderLocked(AppState* state) {
    drawCenteredText("Account locked. Contact admin.", 360, FONT_SIZE_HEADING, colorErrorText());
    int buttonWidth = 240;
    Rectangle backRect = Rectangle{ (float)((WINDOW_WIDTH - buttonWidth) / 2), 440, (float)buttonWidth, 50 };
    if (drawButton(backRect, "Back to Role Selection")) {
        state->loginAttempts = 0;
        resetLoginFields(state);
        state->currentScreen = SCREEN_ROLE_SELECT;
    }
}

void resetBookState(AppState* state) {
    state->bookStep = 0;
    state->bookSpecializationInput.clear();
    state->bookSpecializationActive = false;
    state->bookDoctorIdInput.clear();
    state->bookDoctorIdActive = false;
    state->bookDateInput.clear();
    state->bookDateActive = false;
    state->bookDateAttempts = 0;
    state->bookError.clear();
}

void resetCancelState(AppState* state) {
    state->cancelIdInput.clear();
    state->cancelIdActive = false;
    state->cancelError.clear();
}

void resetPayBillState(AppState* state) {
    state->payBillIdInput.clear();
    state->payBillIdActive = false;
    state->payBillError.clear();
}

void resetTopupState(AppState* state) {
    state->topupAmountInput.clear();
    state->topupAmountActive = false;
    state->topupError.clear();
    state->topupAttempts = 0;
}

void resetAllPatientScreenState(AppState* state) {
    resetBookState(state);
    resetCancelState(state);
    resetPayBillState(state);
    resetTopupState(state);
    state->scrollOffset = 0;
}

void renderPatientMenu(AppState* state) {
    Patient& patient = state->patients[state->currentPatientIndex];
    drawCenteredText("Patient Menu", 50, FONT_SIZE_TITLE, colorTextPrimary());
    MyString welcome = "Welcome, ";
    welcome += patient.getName();
    welcome += "  |  Balance: PKR ";
    welcome += floatToMyString(patient.getBalance());
    drawCenteredText(welcome.c_str(), 110, FONT_SIZE_BODY, colorTextSecondary());
    drawFlashMessage(state, 40, 150);

    int btnWidth = 320;
    int btnHeight = 60;
    int gapX = 40;
    int gapY = 25;
    int cols = 2;
    int totalWidth = cols * btnWidth + gapX;
    int startX = (WINDOW_WIDTH - totalWidth) / 2;
    int startY = 210;

    const char* labels[8] = {
        "Book Appointment", "Cancel Appointment", "View Appointments",
        "View Medical Records", "View Bills", "Pay Bill",
        "Top Up Balance", "Logout"
    };
    int targets[8] = {
        SCREEN_PATIENT_BOOK, SCREEN_PATIENT_CANCEL, SCREEN_PATIENT_VIEW_APPTS,
        SCREEN_PATIENT_VIEW_RECORDS, SCREEN_PATIENT_VIEW_BILLS, SCREEN_PATIENT_PAY_BILL,
        SCREEN_PATIENT_TOPUP, SCREEN_ROLE_SELECT
    };

    for (int i = 0; i < 8; i++) {
        int row = i / cols;
        int col = i % cols;
        Rectangle rect = Rectangle{ (float)(startX + col * (btnWidth + gapX)), (float)(startY + row * (btnHeight + gapY)), (float)btnWidth, (float)btnHeight };
        if (drawButton(rect, labels[i])) {
            if (i == 7) {
                state->currentPatientIndex = -1;
                state->currentScreen = SCREEN_ROLE_SELECT;
            } else {
                resetAllPatientScreenState(state);
                state->currentScreen = targets[i];
            }
        }
    }
}

MyVector<int> findDoctorsBySpecialization(AppState* state, const MyString& spec) {
    MyVector<int> result;
    for (int i = 0; i < state->doctors.size(); i++) {
        if (Validator::caseInsensitiveEqual(state->doctors[i].getSpecialization(), spec)) {
            result.add(i);
        }
    }
    return result;
}

bool isSlotTaken(AppState* state, int doctorId, const MyString& date, const MyString& slot) {
    for (int i = 0; i < state->appointments.size(); i++) {
        Appointment& a = state->appointments[i];
        if (a.getDoctorId() == doctorId && a.getDate() == date && a.getTimeSlot() == slot && a.getStatus() != "cancelled") {
            return true;
        }
    }
    return false;
}

void attemptCheckAvailability(AppState* state) {
    state->bookError.clear();
    if (!Validator::validateId(state->bookDoctorIdInput) || findDoctorIndexById(state, state->bookDoctorIdInput.toInt()) == -1) {
        state->bookError = "Doctor not found. Please enter a valid Doctor ID.";
        return;
    }
    if (!Validator::validateDate(state->bookDateInput)) {
        state->bookDateAttempts++;
        if (state->bookDateAttempts >= 3) {
            setFlashMessage(state, "Too many invalid date attempts. Returning to menu.", true);
            resetAllPatientScreenState(state);
            state->currentScreen = SCREEN_PATIENT_MENU;
            return;
        }
        MyString msg = "Invalid date. Use DD-MM-YYYY with year >= current year. (Attempt ";
        msg += intToMyString(state->bookDateAttempts);
        msg += "/3)";
        state->bookError = msg;
        return;
    }
}

void attemptBookSlot(AppState* state, int slotIndex) {
    state->bookError.clear();
    int doctorId = state->bookDoctorIdInput.toInt();
    MyString date = state->bookDateInput;
    MyString slot = timeSlotByIndex(slotIndex);
    try {
        for (int i = 0; i < state->appointments.size(); i++) {
            Appointment& a = state->appointments[i];
            if (a.getDoctorId() == doctorId && a.getDate() == date && a.getTimeSlot() == slot && a.getStatus() != "cancelled") {
                throw SlotUnavailableException("This time slot is no longer available.");
            }
        }
        int doctorIdx = findDoctorIndexById(state, doctorId);
        float fee = state->doctors[doctorIdx].getFee();
        Patient& patient = state->patients[state->currentPatientIndex];
        if (patient.getBalance() < fee) {
            throw InsufficientFundsException("Insufficient balance to book this appointment. Please top up.");
        }
        patient -= fee;

        int newApptId = state->fileHandler.generateNextId("appointments.txt");
        Appointment newAppt(newApptId, patient.getId(), doctorId, date, slot, MyString("pending"));
        state->fileHandler.appendLine("appointments.txt", buildAppointmentLine(newAppt).c_str());

        int newBillId = state->fileHandler.generateNextId("bills.txt");
        Bill newBill(newBillId, patient.getId(), newApptId, fee, MyString("unpaid"), date);
        state->fileHandler.appendLine("bills.txt", buildBillLine(newBill).c_str());

        state->fileHandler.updateLine("patients.txt", patient.getId(), buildPatientLine(patient).c_str());

        state->appointments.add(newAppt);
        state->bills.add(newBill);

        MyString msg = "Appointment booked successfully. Appointment ID: ";
        msg += intToMyString(newApptId);
        showSuccessMessage(state, msg.c_str());
        resetBookState(state);
        state->currentScreen = SCREEN_PATIENT_MENU;
    } catch (SlotUnavailableException& e) {
        state->bookError = e.what();
    } catch (InsufficientFundsException& e) {
        state->bookError = e.what();
    }
}

void renderPatientBook(AppState* state) {
    drawBackButton(state, SCREEN_PATIENT_MENU);
    drawCenteredText("Book Appointment", 50, FONT_SIZE_TITLE, colorTextPrimary());

    int leftX = 100;
    int fieldWidth = 380;

    drawFieldLabel("Specialization", leftX, 110);
    Rectangle specRect = Rectangle{ (float)leftX, 135, (float)fieldWidth, 45 };
    updateTextField(specRect, &state->bookSpecializationInput, &state->bookSpecializationActive, 50);
    drawTextField(specRect, state->bookSpecializationInput, state->bookSpecializationActive, false, state->cursorTimer);

    int tableY = 200;
    if (!state->bookSpecializationInput.isEmpty()) {
        MyVector<int> matches = findDoctorsBySpecialization(state, state->bookSpecializationInput);
        if (matches.size() == 0) {
            DrawText("No doctors available for that specialization.", leftX, tableY, FONT_SIZE_BODY, colorErrorText());
        } else {
            int colWidths[3] = { 80, 220, 120 };
            MyString headers[3] = { MyString("ID"), MyString("Name"), MyString("Fee") };
            drawTableHeader(leftX, tableY, colWidths, headers, 3);
            for (int i = 0; i < matches.size(); i++) {
                Doctor& d = state->doctors[matches[i]];
                MyString cells[3] = { intToMyString(d.getId()), d.getName(), floatToMyString(d.getFee()) };
                drawTableRow(leftX, tableY + 36 * (i + 1), colWidths, cells, 3, i % 2 == 1);
            }
        }
    }

    int formY = 430;
    drawFieldLabel("Doctor ID", leftX, formY);
    Rectangle docRect = Rectangle{ (float)leftX, (float)(formY + 25), (float)fieldWidth, 45 };
    updateTextField(docRect, &state->bookDoctorIdInput, &state->bookDoctorIdActive, 10);
    drawTextField(docRect, state->bookDoctorIdInput, state->bookDoctorIdActive, false, state->cursorTimer);

    drawFieldLabel("Date (DD-MM-YYYY)", leftX, formY + 85);
    Rectangle dateRect = Rectangle{ (float)leftX, (float)(formY + 110), (float)fieldWidth, 45 };
    updateTextField(dateRect, &state->bookDateInput, &state->bookDateActive, 10);
    drawTextField(dateRect, state->bookDateInput, state->bookDateActive, false, state->cursorTimer);

    Rectangle checkBtn = Rectangle{ (float)leftX, (float)(formY + 170), 220, 45 };
    if (drawButton(checkBtn, "Check Availability")) {
        attemptCheckAvailability(state);
    }

    drawErrorText(state->bookError, leftX, formY + 225);

    bool doctorValid = Validator::validateId(state->bookDoctorIdInput) && findDoctorIndexById(state, state->bookDoctorIdInput.toInt()) != -1;
    bool dateValid = Validator::validateDate(state->bookDateInput);

    if (doctorValid && dateValid) {
        int doctorId = state->bookDoctorIdInput.toInt();
        int rightX = leftX + fieldWidth + 80;
        drawFieldLabel("Available Slots (click to book)", rightX, 110);
        for (int i = 0; i < 8; i++) {
            MyString slot = timeSlotByIndex(i);
            bool taken = isSlotTaken(state, doctorId, state->bookDateInput, slot);
            int row = i / 4;
            int col = i % 4;
            Rectangle slotRect = Rectangle{ (float)(rightX + col * 150), (float)(145 + row * 60), 140, 50 };
            if (taken) {
                DrawRectangleRounded(slotRect, 0.3f, 8, colorPanelAlt());
                int tw = MeasureText(slot.c_str(), FONT_SIZE_BODY);
                DrawText(slot.c_str(), (int)(slotRect.x + (slotRect.width - tw) / 2), (int)(slotRect.y + 14), FONT_SIZE_BODY, colorTextSecondary());
            } else {
                if (drawButton(slotRect, slot.c_str())) {
                    attemptBookSlot(state, i);
                }
            }
        }
    }
}

void attemptCancelAppointment(AppState* state) {
    state->cancelError.clear();
    if (!Validator::validateId(state->cancelIdInput)) {
        state->cancelError = "Please enter a valid appointment ID.";
        return;
    }
    int apptId = state->cancelIdInput.toInt();
    Patient& patient = state->patients[state->currentPatientIndex];
    int apptIdx = -1;
    for (int i = 0; i < state->appointments.size(); i++) {
        Appointment& a = state->appointments[i];
        if (a.getAppointmentId() == apptId && a.getPatientId() == patient.getId() && a.getStatus() == "pending") {
            apptIdx = i;
            break;
        }
    }
    if (apptIdx == -1) {
        state->cancelError = "Invalid appointment ID.";
        return;
    }
    Appointment& appt = state->appointments[apptIdx];
    appt.setStatus("cancelled");
    state->fileHandler.updateLine("appointments.txt", appt.getAppointmentId(), buildAppointmentLine(appt).c_str());

    int billIdx = -1;
    for (int i = 0; i < state->bills.size(); i++) {
        if (state->bills[i].getAppointmentId() == apptId && state->bills[i].getPatientId() == patient.getId()) {
            billIdx = i;
            break;
        }
    }
    float refund = 0.0f;
    if (billIdx != -1) {
        refund = state->bills[billIdx].getAmount();
        state->bills[billIdx].setStatus("cancelled");
        state->fileHandler.updateLine("bills.txt", state->bills[billIdx].getBillId(), buildBillLine(state->bills[billIdx]).c_str());
    }
    patient += refund;
    state->fileHandler.updateLine("patients.txt", patient.getId(), buildPatientLine(patient).c_str());

    MyString msg = "Appointment cancelled. PKR ";
    msg += floatToMyString(refund);
    msg += " refunded to your balance.";
    showSuccessMessage(state, msg.c_str());
    state->cancelIdInput.clear();
}

void renderPatientCancel(AppState* state) {
    drawBackButton(state, SCREEN_PATIENT_MENU);
    drawCenteredText("Cancel Appointment", 50, FONT_SIZE_TITLE, colorTextPrimary());
    drawSuccessMessage(state, 140, 105);

    Patient& patient = state->patients[state->currentPatientIndex];
    MyVector<Appointment> pending;
    for (int i = 0; i < state->appointments.size(); i++) {
        if (state->appointments[i].getPatientId() == patient.getId() && state->appointments[i].getStatus() == "pending") {
            pending.add(state->appointments[i]);
        }
    }

    int leftX = 100;
    int tableY = 150;
    if (pending.size() == 0) {
        DrawText("You have no pending appointments.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
    } else {
        int colWidths[5] = { 80, 240, 160, 120, 140 };
        MyString headers[5] = { MyString("ID"), MyString("Doctor"), MyString("Date"), MyString("Time"), MyString("Status") };
        drawTableHeader(leftX, tableY, colWidths, headers, 5);
        for (int i = 0; i < pending.size(); i++) {
            Appointment& a = pending[i];
            MyString cells[5] = { intToMyString(a.getAppointmentId()), getDoctorName(state, a.getDoctorId()), a.getDate(), a.getTimeSlot(), a.getStatus() };
            drawTableRow(leftX, tableY + 36 * (i + 1), colWidths, cells, 5, i % 2 == 1);
        }
    }

    int formY = tableY + 36 * (pending.size() + 1) + 60;
    drawFieldLabel("Appointment ID to cancel", leftX, formY);
    Rectangle idRect = Rectangle{ (float)leftX, (float)(formY + 25), 300, 45 };
    updateTextField(idRect, &state->cancelIdInput, &state->cancelIdActive, 10);
    drawTextField(idRect, state->cancelIdInput, state->cancelIdActive, false, state->cursorTimer);
    drawErrorText(state->cancelError, leftX, formY + 80);

    Rectangle cancelBtn = Rectangle{ (float)leftX, (float)(formY + 120), 220, 45 };
    if (drawButton(cancelBtn, "Cancel Appointment")) {
        attemptCancelAppointment(state);
    }
}

void renderPatientViewAppts(AppState* state) {
    drawBackButton(state, SCREEN_PATIENT_MENU);
    drawCenteredText("My Appointments", 50, FONT_SIZE_TITLE, colorTextPrimary());

    Patient& patient = state->patients[state->currentPatientIndex];
    MyVector<Appointment> list;
    for (int i = 0; i < state->appointments.size(); i++) {
        if (state->appointments[i].getPatientId() == patient.getId()) {
            list.add(state->appointments[i]);
        }
    }
    sortAppointmentsByDateAsc(&list);

    int leftX = 100;
    int tableY = 130;
    int visibleHeight = WINDOW_HEIGHT - tableY - 40;

    if (list.size() == 0) {
        DrawText("No appointments found.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
        return;
    }

    int maxScroll = computeMaxScroll(list.size(), visibleHeight - 36, 36);
    updateScroll(state, maxScroll);

    int colWidths[6] = { 80, 220, 200, 140, 120, 140 };
    MyString headers[6] = { MyString("ID"), MyString("Doctor"), MyString("Specialization"), MyString("Date"), MyString("Time"), MyString("Status") };
    drawTableHeader(leftX, tableY, colWidths, headers, 6);

    BeginScissorMode(leftX, tableY + 36, 1080, visibleHeight - 36);
    for (int i = 0; i < list.size(); i++) {
        int rowY = tableY + 36 + i * 36 - state->scrollOffset;
        if (rowY < tableY + 36 - 36 || rowY > WINDOW_HEIGHT) continue;
        Appointment& a = list[i];
        MyString cells[6] = { intToMyString(a.getAppointmentId()), getDoctorName(state, a.getDoctorId()), getDoctorSpecialization(state, a.getDoctorId()), a.getDate(), a.getTimeSlot(), a.getStatus() };
        drawTableRow(leftX, rowY, colWidths, cells, 6, i % 2 == 1);
    }
    EndScissorMode();
}

void renderPatientViewRecords(AppState* state) {
    drawBackButton(state, SCREEN_PATIENT_MENU);
    drawCenteredText("My Medical Records", 50, FONT_SIZE_TITLE, colorTextPrimary());

    Patient& patient = state->patients[state->currentPatientIndex];
    MyVector<Prescription> list;
    for (int i = 0; i < state->prescriptions.size(); i++) {
        if (state->prescriptions[i].getPatientId() == patient.getId()) {
            list.add(state->prescriptions[i]);
        }
    }
    sortPrescriptionsByDateDesc(&list);

    int leftX = 100;
    int tableY = 130;
    int visibleHeight = WINDOW_HEIGHT - tableY - 40;

    if (list.size() == 0) {
        DrawText("No medical records found.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
        return;
    }

    int maxScroll = computeMaxScroll(list.size(), visibleHeight - 36, 36);
    updateScroll(state, maxScroll);

    int colWidths[4] = { 120, 220, 460, 260 };
    MyString headers[4] = { MyString("Date"), MyString("Doctor"), MyString("Medicines"), MyString("Notes") };
    drawTableHeader(leftX, tableY, colWidths, headers, 4);

    BeginScissorMode(leftX, tableY + 36, 1080, visibleHeight - 36);
    for (int i = 0; i < list.size(); i++) {
        int rowY = tableY + 36 + i * 36 - state->scrollOffset;
        if (rowY < tableY + 36 - 36 || rowY > WINDOW_HEIGHT) continue;
        Prescription& p = list[i];
        MyString cells[4] = { p.getDate(), getDoctorName(state, p.getDoctorId()), p.getMedicines(), p.getNotes() };
        drawTableRow(leftX, rowY, colWidths, cells, 4, i % 2 == 1);
    }
    EndScissorMode();
}

void renderPatientViewBills(AppState* state) {
    drawBackButton(state, SCREEN_PATIENT_MENU);
    drawCenteredText("My Bills", 50, FONT_SIZE_TITLE, colorTextPrimary());

    Patient& patient = state->patients[state->currentPatientIndex];
    MyVector<Bill> list;
    float outstanding = 0.0f;
    for (int i = 0; i < state->bills.size(); i++) {
        if (state->bills[i].getPatientId() == patient.getId()) {
            list.add(state->bills[i]);
            if (state->bills[i].getStatus() == "unpaid") {
                outstanding += state->bills[i].getAmount();
            }
        }
    }

    int leftX = 100;
    int tableY = 130;

    if (list.size() == 0) {
        DrawText("No bills found.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
        return;
    }

    int colWidths[5] = { 100, 140, 140, 140, 140 };
    MyString headers[5] = { MyString("Bill ID"), MyString("Appt ID"), MyString("Amount"), MyString("Status"), MyString("Date") };
    drawTableHeader(leftX, tableY, colWidths, headers, 5);
    for (int i = 0; i < list.size(); i++) {
        Bill& b = list[i];
        MyString cells[5] = { intToMyString(b.getBillId()), intToMyString(b.getAppointmentId()), floatToMyString(b.getAmount()), b.getStatus(), b.getDate() };
        drawTableRow(leftX, tableY + 36 * (i + 1), colWidths, cells, 5, i % 2 == 1);
    }

    MyString totalText = "Total Outstanding (Unpaid): PKR ";
    totalText += floatToMyString(outstanding);
    DrawText(totalText.c_str(), leftX, tableY + 36 * (list.size() + 1) + 30, FONT_SIZE_HEADING, colorTextPrimary());
}

void attemptPayBill(AppState* state) {
    state->payBillError.clear();
    if (!Validator::validateId(state->payBillIdInput)) {
        state->payBillError = "Please enter a valid bill ID.";
        return;
    }
    int billId = state->payBillIdInput.toInt();
    Patient& patient = state->patients[state->currentPatientIndex];
    int billIdx = -1;
    for (int i = 0; i < state->bills.size(); i++) {
        if (state->bills[i].getBillId() == billId && state->bills[i].getPatientId() == patient.getId() && state->bills[i].getStatus() == "unpaid") {
            billIdx = i;
            break;
        }
    }
    if (billIdx == -1) {
        state->payBillError = "Invalid bill ID.";
        return;
    }
    try {
        Bill& bill = state->bills[billIdx];
        if (patient.getBalance() < bill.getAmount()) {
            throw InsufficientFundsException("Insufficient balance to pay this bill.");
        }
        patient -= bill.getAmount();
        bill.setStatus("paid");
        state->fileHandler.updateLine("bills.txt", bill.getBillId(), buildBillLine(bill).c_str());
        state->fileHandler.updateLine("patients.txt", patient.getId(), buildPatientLine(patient).c_str());

        MyString msg = "Bill paid successfully. Remaining balance: PKR ";
        msg += floatToMyString(patient.getBalance());
        showSuccessMessage(state, msg.c_str());
        state->payBillIdInput.clear();
    } catch (InsufficientFundsException& e) {
        state->payBillError = e.what();
    }
}

void renderPatientPayBill(AppState* state) {
    drawBackButton(state, SCREEN_PATIENT_MENU);
    drawCenteredText("Pay Bill", 50, FONT_SIZE_TITLE, colorTextPrimary());
    drawSuccessMessage(state, 140, 105);

    Patient& patient = state->patients[state->currentPatientIndex];
    MyVector<Bill> unpaid;
    for (int i = 0; i < state->bills.size(); i++) {
        if (state->bills[i].getPatientId() == patient.getId() && state->bills[i].getStatus() == "unpaid") {
            unpaid.add(state->bills[i]);
        }
    }

    int leftX = 100;
    int tableY = 150;
    if (unpaid.size() == 0) {
        DrawText("No unpaid bills.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
    } else {
        int colWidths[4] = { 100, 140, 140, 140 };
        MyString headers[4] = { MyString("Bill ID"), MyString("Appt ID"), MyString("Amount"), MyString("Date") };
        drawTableHeader(leftX, tableY, colWidths, headers, 4);
        for (int i = 0; i < unpaid.size(); i++) {
            Bill& b = unpaid[i];
            MyString cells[4] = { intToMyString(b.getBillId()), intToMyString(b.getAppointmentId()), floatToMyString(b.getAmount()), b.getDate() };
            drawTableRow(leftX, tableY + 36 * (i + 1), colWidths, cells, 4, i % 2 == 1);
        }
    }

    int formY = tableY + 36 * (unpaid.size() + 1) + 60;
    drawFieldLabel("Bill ID to pay", leftX, formY);
    Rectangle idRect = Rectangle{ (float)leftX, (float)(formY + 25), 300, 45 };
    updateTextField(idRect, &state->payBillIdInput, &state->payBillIdActive, 10);
    drawTextField(idRect, state->payBillIdInput, state->payBillIdActive, false, state->cursorTimer);
    drawErrorText(state->payBillError, leftX, formY + 80);

    Rectangle payBtn = Rectangle{ (float)leftX, (float)(formY + 120), 200, 45 };
    if (drawButton(payBtn, "Pay Bill")) {
        attemptPayBill(state);
    }
}

void attemptTopup(AppState* state) {
    state->topupError.clear();
    if (!Validator::validatePositiveFloat(state->topupAmountInput)) {
        state->topupAttempts++;
        if (state->topupAttempts >= 3) {
            setFlashMessage(state, "Too many invalid amounts entered. Returning to menu.", true);
            resetTopupState(state);
            state->currentScreen = SCREEN_PATIENT_MENU;
            return;
        }
        MyString msg = "Invalid amount. Enter a positive number. (Attempt ";
        msg += intToMyString(state->topupAttempts);
        msg += "/3)";
        state->topupError = msg;
        return;
    }
    float amount = state->topupAmountInput.toFloat();
    Patient& patient = state->patients[state->currentPatientIndex];
    patient += amount;
    state->fileHandler.updateLine("patients.txt", patient.getId(), buildPatientLine(patient).c_str());

    MyString msg = "Balance updated. New balance: PKR ";
    msg += floatToMyString(patient.getBalance());
    showSuccessMessage(state, msg.c_str());
    resetTopupState(state);
}

void renderPatientTopup(AppState* state) {
    drawBackButton(state, SCREEN_PATIENT_MENU);
    drawCenteredText("Top Up Balance", 50, FONT_SIZE_TITLE, colorTextPrimary());
    drawSuccessMessage(state, 140, 105);

    Patient& patient = state->patients[state->currentPatientIndex];
    MyString balanceText = "Current Balance: PKR ";
    balanceText += floatToMyString(patient.getBalance());
    DrawText(balanceText.c_str(), 100, 150, FONT_SIZE_HEADING, colorTextPrimary());

    int leftX = 100;
    int formY = 230;
    drawFieldLabel("Amount to add (PKR)", leftX, formY);
    Rectangle amountRect = Rectangle{ (float)leftX, (float)(formY + 25), 300, 45 };
    updateTextField(amountRect, &state->topupAmountInput, &state->topupAmountActive, 12);
    drawTextField(amountRect, state->topupAmountInput, state->topupAmountActive, false, state->cursorTimer);
    drawErrorText(state->topupError, leftX, formY + 80);

    Rectangle topupBtn = Rectangle{ (float)leftX, (float)(formY + 120), 200, 45 };
    if (drawButton(topupBtn, "Top Up")) {
        attemptTopup(state);
    }
}

void resetDoctorCompleteState(AppState* state) {
    state->doctorApptIdInput.clear();
    state->doctorApptIdActive = false;
    state->doctorError.clear();
}

void resetPrescribeState(AppState* state) {
    state->doctorApptIdInput.clear();
    state->doctorApptIdActive = false;
    state->doctorError.clear();
    state->prescribeMedicinesInput.clear();
    state->prescribeMedicinesActive = false;
    state->prescribeNotesInput.clear();
    state->prescribeNotesActive = false;
}

void resetHistoryState(AppState* state) {
    state->historyPatientIdInput.clear();
    state->historyPatientIdActive = false;
    state->historyError.clear();
}

void resetAllDoctorScreenState(AppState* state) {
    resetDoctorCompleteState(state);
    resetPrescribeState(state);
    resetHistoryState(state);
    state->scrollOffset = 0;
}

void renderDoctorMenu(AppState* state) {
    Doctor& doctor = state->doctors[state->currentDoctorIndex];
    drawCenteredText("Doctor Menu", 50, FONT_SIZE_TITLE, colorTextPrimary());
    MyString welcome = "Welcome, Dr. ";
    welcome += doctor.getName();
    welcome += "  |  ";
    welcome += doctor.getSpecialization();
    drawCenteredText(welcome.c_str(), 110, FONT_SIZE_BODY, colorTextSecondary());
    drawFlashMessage(state, 40, 150);

    int btnWidth = 320;
    int btnHeight = 60;
    int gapX = 40;
    int gapY = 25;
    int cols = 2;
    int totalWidth = cols * btnWidth + gapX;
    int startX = (WINDOW_WIDTH - totalWidth) / 2;
    int startY = 210;

    const char* labels[6] = {
        "Today's Appointments", "Mark Complete", "Mark No-show",
        "Write Prescription", "Patient Medical History", "Logout"
    };
    int targets[6] = {
        SCREEN_DOCTOR_TODAY, SCREEN_DOCTOR_COMPLETE, SCREEN_DOCTOR_NOSHOW,
        SCREEN_DOCTOR_PRESCRIBE, SCREEN_DOCTOR_HISTORY, SCREEN_ROLE_SELECT
    };

    for (int i = 0; i < 6; i++) {
        int row = i / cols;
        int col = i % cols;
        Rectangle rect = Rectangle{ (float)(startX + col * (btnWidth + gapX)), (float)(startY + row * (btnHeight + gapY)), (float)btnWidth, (float)btnHeight };
        if (drawButton(rect, labels[i])) {
            if (i == 5) {
                state->currentDoctorIndex = -1;
                state->currentScreen = SCREEN_ROLE_SELECT;
            } else {
                resetAllDoctorScreenState(state);
                state->currentScreen = targets[i];
            }
        }
    }
}

void renderDoctorToday(AppState* state) {
    drawBackButton(state, SCREEN_DOCTOR_MENU);
    drawCenteredText("Today's Appointments", 50, FONT_SIZE_TITLE, colorTextPrimary());

    Doctor& doctor = state->doctors[state->currentDoctorIndex];
    MyString today = getCurrentDateString();
    MyVector<Appointment> list;
    for (int i = 0; i < state->appointments.size(); i++) {
        if (state->appointments[i].getDoctorId() == doctor.getId() && state->appointments[i].getDate() == today) {
            list.add(state->appointments[i]);
        }
    }
    sortAppointmentsByTimeSlotAsc(&list);

    int leftX = 100;
    int tableY = 130;
    if (list.size() == 0) {
        DrawText("No appointments scheduled for today.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
        return;
    }

    int colWidths[4] = { 100, 280, 140, 160 };
    MyString headers[4] = { MyString("Appt ID"), MyString("Patient"), MyString("Time"), MyString("Status") };
    drawTableHeader(leftX, tableY, colWidths, headers, 4);
    for (int i = 0; i < list.size(); i++) {
        Appointment& a = list[i];
        MyString cells[4] = { intToMyString(a.getAppointmentId()), getPatientName(state, a.getPatientId()), a.getTimeSlot(), a.getStatus() };
        drawTableRow(leftX, tableY + 36 * (i + 1), colWidths, cells, 4, i % 2 == 1);
    }
}

void attemptMarkComplete(AppState* state) {
    state->doctorError.clear();
    if (!Validator::validateId(state->doctorApptIdInput)) {
        state->doctorError = "Please enter a valid appointment ID.";
        return;
    }
    int apptId = state->doctorApptIdInput.toInt();
    Doctor& doctor = state->doctors[state->currentDoctorIndex];
    MyString today = getCurrentDateString();
    int idx = -1;
    for (int i = 0; i < state->appointments.size(); i++) {
        Appointment& a = state->appointments[i];
        if (a.getAppointmentId() == apptId && a.getDoctorId() == doctor.getId() && a.getStatus() == "pending" && a.getDate() == today) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        state->doctorError = "Invalid appointment ID.";
        return;
    }
    state->appointments[idx].setStatus("completed");
    state->fileHandler.updateLine("appointments.txt", apptId, buildAppointmentLine(state->appointments[idx]).c_str());
    showSuccessMessage(state, "Appointment marked as completed.");
    state->doctorApptIdInput.clear();
}

void renderDoctorComplete(AppState* state) {
    drawBackButton(state, SCREEN_DOCTOR_MENU);
    drawCenteredText("Mark Appointment Complete", 50, FONT_SIZE_TITLE, colorTextPrimary());
    drawSuccessMessage(state, 140, 105);

    Doctor& doctor = state->doctors[state->currentDoctorIndex];
    MyString today = getCurrentDateString();
    MyVector<Appointment> pending;
    for (int i = 0; i < state->appointments.size(); i++) {
        Appointment& a = state->appointments[i];
        if (a.getDoctorId() == doctor.getId() && a.getStatus() == "pending" && a.getDate() == today) {
            pending.add(a);
        }
    }

    int leftX = 100;
    int tableY = 150;
    if (pending.size() == 0) {
        DrawText("No pending appointments for today.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
    } else {
        int colWidths[3] = { 100, 280, 140 };
        MyString headers[3] = { MyString("Appt ID"), MyString("Patient"), MyString("Time") };
        drawTableHeader(leftX, tableY, colWidths, headers, 3);
        for (int i = 0; i < pending.size(); i++) {
            Appointment& a = pending[i];
            MyString cells[3] = { intToMyString(a.getAppointmentId()), getPatientName(state, a.getPatientId()), a.getTimeSlot() };
            drawTableRow(leftX, tableY + 36 * (i + 1), colWidths, cells, 3, i % 2 == 1);
        }
    }

    int formY = tableY + 36 * (pending.size() + 1) + 60;
    drawFieldLabel("Appointment ID", leftX, formY);
    Rectangle idRect = Rectangle{ (float)leftX, (float)(formY + 25), 300, 45 };
    updateTextField(idRect, &state->doctorApptIdInput, &state->doctorApptIdActive, 10);
    drawTextField(idRect, state->doctorApptIdInput, state->doctorApptIdActive, false, state->cursorTimer);
    drawErrorText(state->doctorError, leftX, formY + 80);

    Rectangle btn = Rectangle{ (float)leftX, (float)(formY + 120), 220, 45 };
    if (drawButton(btn, "Mark Complete")) {
        attemptMarkComplete(state);
    }
}

void attemptMarkNoShow(AppState* state) {
    state->doctorError.clear();
    if (!Validator::validateId(state->doctorApptIdInput)) {
        state->doctorError = "Please enter a valid appointment ID.";
        return;
    }
    int apptId = state->doctorApptIdInput.toInt();
    Doctor& doctor = state->doctors[state->currentDoctorIndex];
    MyString today = getCurrentDateString();
    int idx = -1;
    for (int i = 0; i < state->appointments.size(); i++) {
        Appointment& a = state->appointments[i];
        if (a.getAppointmentId() == apptId && a.getDoctorId() == doctor.getId() && a.getStatus() == "pending" && a.getDate() == today) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        state->doctorError = "Invalid appointment ID.";
        return;
    }
    state->appointments[idx].setStatus("noshow");
    state->fileHandler.updateLine("appointments.txt", apptId, buildAppointmentLine(state->appointments[idx]).c_str());

    for (int i = 0; i < state->bills.size(); i++) {
        if (state->bills[i].getAppointmentId() == apptId) {
            state->bills[i].setStatus("cancelled");
            state->fileHandler.updateLine("bills.txt", state->bills[i].getBillId(), buildBillLine(state->bills[i]).c_str());
            break;
        }
    }
    showSuccessMessage(state, "Appointment marked as no-show.");
    state->doctorApptIdInput.clear();
}

void renderDoctorNoshow(AppState* state) {
    drawBackButton(state, SCREEN_DOCTOR_MENU);
    drawCenteredText("Mark Appointment No-show", 50, FONT_SIZE_TITLE, colorTextPrimary());
    drawSuccessMessage(state, 140, 105);

    Doctor& doctor = state->doctors[state->currentDoctorIndex];
    MyString today = getCurrentDateString();
    MyVector<Appointment> pending;
    for (int i = 0; i < state->appointments.size(); i++) {
        Appointment& a = state->appointments[i];
        if (a.getDoctorId() == doctor.getId() && a.getStatus() == "pending" && a.getDate() == today) {
            pending.add(a);
        }
    }

    int leftX = 100;
    int tableY = 150;
    if (pending.size() == 0) {
        DrawText("No pending appointments for today.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
    } else {
        int colWidths[3] = { 100, 280, 140 };
        MyString headers[3] = { MyString("Appt ID"), MyString("Patient"), MyString("Time") };
        drawTableHeader(leftX, tableY, colWidths, headers, 3);
        for (int i = 0; i < pending.size(); i++) {
            Appointment& a = pending[i];
            MyString cells[3] = { intToMyString(a.getAppointmentId()), getPatientName(state, a.getPatientId()), a.getTimeSlot() };
            drawTableRow(leftX, tableY + 36 * (i + 1), colWidths, cells, 3, i % 2 == 1);
        }
    }

    int formY = tableY + 36 * (pending.size() + 1) + 60;
    drawFieldLabel("Appointment ID", leftX, formY);
    Rectangle idRect = Rectangle{ (float)leftX, (float)(formY + 25), 300, 45 };
    updateTextField(idRect, &state->doctorApptIdInput, &state->doctorApptIdActive, 10);
    drawTextField(idRect, state->doctorApptIdInput, state->doctorApptIdActive, false, state->cursorTimer);
    drawErrorText(state->doctorError, leftX, formY + 80);

    Rectangle btn = Rectangle{ (float)leftX, (float)(formY + 120), 220, 45 };
    if (drawButton(btn, "Mark No-show")) {
        attemptMarkNoShow(state);
    }
}

bool prescriptionExistsForAppointment(AppState* state, int apptId) {
    for (int i = 0; i < state->prescriptions.size(); i++) {
        if (state->prescriptions[i].getAppointmentId() == apptId) return true;
    }
    return false;
}

void attemptWritePrescription(AppState* state) {
    state->doctorError.clear();
    if (!Validator::validateId(state->doctorApptIdInput)) {
        state->doctorError = "Please enter a valid appointment ID.";
        return;
    }
    int apptId = state->doctorApptIdInput.toInt();
    Doctor& doctor = state->doctors[state->currentDoctorIndex];
    int apptIdx = -1;
    for (int i = 0; i < state->appointments.size(); i++) {
        Appointment& a = state->appointments[i];
        if (a.getAppointmentId() == apptId && a.getDoctorId() == doctor.getId() && a.getStatus() == "completed") {
            apptIdx = i;
            break;
        }
    }
    if (apptIdx == -1) {
        state->doctorError = "Invalid appointment ID or appointment not completed.";
        return;
    }
    if (prescriptionExistsForAppointment(state, apptId)) {
        state->doctorError = "Prescription already written for this appointment.";
        return;
    }
    if (state->prescribeMedicinesInput.isEmpty()) {
        state->doctorError = "Please enter medicines.";
        return;
    }
    Appointment& appt = state->appointments[apptIdx];
    int newId = state->fileHandler.generateNextId("prescriptions.txt");
    Prescription newPres(newId, apptId, appt.getPatientId(), doctor.getId(), getCurrentDateString(), state->prescribeMedicinesInput, state->prescribeNotesInput);
    state->fileHandler.appendLine("prescriptions.txt", buildPrescriptionLine(newPres).c_str());
    state->prescriptions.add(newPres);

    showSuccessMessage(state, "Prescription saved.");
    resetPrescribeState(state);
}

void renderDoctorPrescribe(AppState* state) {
    drawBackButton(state, SCREEN_DOCTOR_MENU);
    drawCenteredText("Write Prescription", 50, FONT_SIZE_TITLE, colorTextPrimary());
    drawSuccessMessage(state, 140, 105);

    int leftX = 100;
    int formY = 150;
    drawFieldLabel("Appointment ID (must be completed)", leftX, formY);
    Rectangle idRect = Rectangle{ (float)leftX, (float)(formY + 25), 300, 45 };
    updateTextField(idRect, &state->doctorApptIdInput, &state->doctorApptIdActive, 10);
    drawTextField(idRect, state->doctorApptIdInput, state->doctorApptIdActive, false, state->cursorTimer);

    drawFieldLabel("Medicines (max 500 chars)", leftX, formY + 90);
    Rectangle medRect = Rectangle{ (float)leftX, (float)(formY + 115), 1080, 45 };
    updateTextField(medRect, &state->prescribeMedicinesInput, &state->prescribeMedicinesActive, 500);
    drawTextField(medRect, state->prescribeMedicinesInput, state->prescribeMedicinesActive, false, state->cursorTimer);

    drawFieldLabel("Notes (max 300 chars)", leftX, formY + 180);
    Rectangle notesRect = Rectangle{ (float)leftX, (float)(formY + 205), 1080, 45 };
    updateTextField(notesRect, &state->prescribeNotesInput, &state->prescribeNotesActive, 300);
    drawTextField(notesRect, state->prescribeNotesInput, state->prescribeNotesActive, false, state->cursorTimer);

    drawErrorText(state->doctorError, leftX, formY + 270);

    Rectangle btn = Rectangle{ (float)leftX, (float)(formY + 320), 220, 45 };
    if (drawButton(btn, "Save Prescription")) {
        attemptWritePrescription(state);
    }
}

bool doctorHasCompletedAppointmentWithPatient(AppState* state, int doctorId, int patientId) {
    for (int i = 0; i < state->appointments.size(); i++) {
        Appointment& a = state->appointments[i];
        if (a.getDoctorId() == doctorId && a.getPatientId() == patientId && a.getStatus() == "completed") {
            return true;
        }
    }
    return false;
}

void attemptViewHistory(AppState* state) {
    state->historyError.clear();
    if (!Validator::validateId(state->historyPatientIdInput)) {
        state->historyError = "Please enter a valid patient ID.";
        return;
    }
    int patientId = state->historyPatientIdInput.toInt();
    Doctor& doctor = state->doctors[state->currentDoctorIndex];
    if (findPatientIndexById(state, patientId) == -1) {
        state->historyError = "Patient not found.";
        return;
    }
    if (!doctorHasCompletedAppointmentWithPatient(state, doctor.getId(), patientId)) {
        state->historyError = "Access denied. You can only view records of your own patients.";
        return;
    }
    state->historyError.clear();
}

void renderDoctorHistory(AppState* state) {
    drawBackButton(state, SCREEN_DOCTOR_MENU);
    drawCenteredText("Patient Medical History", 50, FONT_SIZE_TITLE, colorTextPrimary());

    int leftX = 100;
    int formY = 110;
    drawFieldLabel("Patient ID", leftX, formY);
    Rectangle idRect = Rectangle{ (float)leftX, (float)(formY + 25), 300, 45 };
    updateTextField(idRect, &state->historyPatientIdInput, &state->historyPatientIdActive, 10);
    drawTextField(idRect, state->historyPatientIdInput, state->historyPatientIdActive, false, state->cursorTimer);

    Rectangle btn = Rectangle{ (float)(leftX + 320), (float)(formY + 25), 160, 45 };
    if (drawButton(btn, "View")) {
        attemptViewHistory(state);
    }
    drawErrorText(state->historyError, leftX, formY + 80);

    Doctor& doctor = state->doctors[state->currentDoctorIndex];
    if (Validator::validateId(state->historyPatientIdInput)) {
        int patientId = state->historyPatientIdInput.toInt();
        if (findPatientIndexById(state, patientId) != -1 && doctorHasCompletedAppointmentWithPatient(state, doctor.getId(), patientId)) {
            MyVector<Prescription> list;
            for (int i = 0; i < state->prescriptions.size(); i++) {
                if (state->prescriptions[i].getPatientId() == patientId && state->prescriptions[i].getDoctorId() == doctor.getId()) {
                    list.add(state->prescriptions[i]);
                }
            }
            sortPrescriptionsByDateDesc(&list);

            int tableY = formY + 130;
            if (list.size() == 0) {
                DrawText("No medical records found.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
            } else {
                int colWidths[3] = { 140, 480, 460 };
                MyString headers[3] = { MyString("Date"), MyString("Medicines"), MyString("Notes") };
                drawTableHeader(leftX, tableY, colWidths, headers, 3);
                for (int i = 0; i < list.size(); i++) {
                    Prescription& p = list[i];
                    MyString cells[3] = { p.getDate(), p.getMedicines(), p.getNotes() };
                    drawTableRow(leftX, tableY + 36 * (i + 1), colWidths, cells, 3, i % 2 == 1);
                }
            }
        }
    }
}

void resetAddDoctorState(AppState* state) {
    state->addDocNameInput.clear();
    state->addDocNameActive = false;
    state->addDocSpecInput.clear();
    state->addDocSpecActive = false;
    state->addDocContactInput.clear();
    state->addDocContactActive = false;
    state->addDocPasswordInput.clear();
    state->addDocPasswordActive = false;
    state->addDocFeeInput.clear();
    state->addDocFeeActive = false;
    state->addDocError.clear();
}

void resetRemoveDoctorState(AppState* state) {
    state->removeDocIdInput.clear();
    state->removeDocIdActive = false;
    state->removeDocError.clear();
}

void resetDischargeState(AppState* state) {
    state->dischargeIdInput.clear();
    state->dischargeIdActive = false;
    state->dischargeError.clear();
}

void resetAllAdminScreenState(AppState* state) {
    resetAddDoctorState(state);
    resetRemoveDoctorState(state);
    resetDischargeState(state);
    state->scrollOffset = 0;
}

void attemptAddDoctor(AppState* state) {
    state->addDocError.clear();
    if (state->addDocNameInput.isEmpty()) {
        state->addDocError = "Name is required.";
        return;
    }
    if (state->addDocSpecInput.isEmpty()) {
        state->addDocError = "Specialization is required.";
        return;
    }
    if (!Validator::validateContact(state->addDocContactInput)) {
        state->addDocError = "Contact must be exactly 11 digits.";
        return;
    }
    if (!Validator::validatePassword(state->addDocPasswordInput)) {
        state->addDocError = "Password must be at least 6 characters.";
        return;
    }
    if (!Validator::validatePositiveFloat(state->addDocFeeInput)) {
        state->addDocError = "Fee must be a positive number.";
        return;
    }

    MyString name = state->addDocNameInput;
    if (name.length() > 50) name = name.substr(0, 50);
    MyString spec = state->addDocSpecInput;
    if (spec.length() > 50) spec = spec.substr(0, 50);

    int newId = state->fileHandler.generateNextId("doctors.txt");
    Doctor newDoctor(newId, name, spec, state->addDocContactInput, state->addDocPasswordInput, state->addDocFeeInput.toFloat());
    state->fileHandler.appendLine("doctors.txt", buildDoctorLine(newDoctor).c_str());
    state->doctors.add(newDoctor);

    MyString msg = "Doctor added successfully. ID: ";
    msg += intToMyString(newId);
    resetAddDoctorState(state);
    showSuccessMessage(state, msg.c_str());
}

void attemptRemoveDoctor(AppState* state) {
    state->removeDocError.clear();
    if (!Validator::validateId(state->removeDocIdInput)) {
        state->removeDocError = "Please enter a valid doctor ID.";
        return;
    }

    int doctorId = state->removeDocIdInput.toInt();
    int idx = findDoctorIndexById(state, doctorId);
    if (idx == -1) {
        state->removeDocError = "Doctor not found.";
        return;
    }

    for (int i = 0; i < state->appointments.size(); i++) {
        if (state->appointments[i].getDoctorId() == doctorId && state->appointments[i].getStatus() == "pending") {
            state->removeDocError = "Cannot remove a doctor with pending appointments.";
            return;
        }
    }

    state->fileHandler.deleteLine("doctors.txt", doctorId);
    state->doctors.removeAt(idx);
    state->removeDocIdInput.clear();
    showSuccessMessage(state, "Doctor removed successfully.");
}

void attemptDischarge(AppState* state) {
    state->dischargeError.clear();
    if (!Validator::validateId(state->dischargeIdInput)) {
        state->dischargeError = "Please enter a valid patient ID.";
        return;
    }

    int patientId = state->dischargeIdInput.toInt();
    int patientIdx = findPatientIndexById(state, patientId);
    if (patientIdx == -1) {
        state->dischargeError = "Patient not found.";
        return;
    }

    for (int i = 0; i < state->bills.size(); i++) {
        if (state->bills[i].getPatientId() == patientId && state->bills[i].getStatus() == "unpaid") {
            state->dischargeError = "Cannot discharge a patient with unpaid bills.";
            return;
        }
    }

    for (int i = 0; i < state->appointments.size(); i++) {
        if (state->appointments[i].getPatientId() == patientId && state->appointments[i].getStatus() == "pending") {
            state->dischargeError = "Cannot discharge a patient with pending appointments.";
            return;
        }
    }

    Patient patient = state->patients[patientIdx];
    MyString patientLine = "PATIENT,";
    patientLine += buildPatientLine(patient);
    state->fileHandler.appendLine("discharged.txt", patientLine.c_str());

    for (int i = state->appointments.size() - 1; i >= 0; i--) {
        if (state->appointments[i].getPatientId() == patientId) {
            MyString line = "APPOINTMENT,";
            line += buildAppointmentLine(state->appointments[i]);
            state->fileHandler.appendLine("discharged.txt", line.c_str());
            state->fileHandler.deleteLine("appointments.txt", state->appointments[i].getAppointmentId());
            state->appointments.removeAt(i);
        }
    }

    for (int i = state->prescriptions.size() - 1; i >= 0; i--) {
        if (state->prescriptions[i].getPatientId() == patientId) {
            MyString line = "PRESCRIPTION,";
            line += buildPrescriptionLine(state->prescriptions[i]);
            state->fileHandler.appendLine("discharged.txt", line.c_str());
            state->fileHandler.deleteLine("prescriptions.txt", state->prescriptions[i].getPrescriptionId());
            state->prescriptions.removeAt(i);
        }
    }

    for (int i = state->bills.size() - 1; i >= 0; i--) {
        if (state->bills[i].getPatientId() == patientId) {
            MyString line = "BILL,";
            line += buildBillLine(state->bills[i]);
            state->fileHandler.appendLine("discharged.txt", line.c_str());
            state->fileHandler.deleteLine("bills.txt", state->bills[i].getBillId());
            state->bills.removeAt(i);
        }
    }

    state->fileHandler.deleteLine("patients.txt", patientId);
    state->patients.removeAt(patientIdx);

    state->dischargeIdInput.clear();
    showSuccessMessage(state, "Patient discharged and archived successfully.");
}

int countUnpaidBills(AppState* state, int patientId) {
    int count = 0;
    for (int i = 0; i < state->bills.size(); i++) {
        if (state->bills[i].getPatientId() == patientId && state->bills[i].getStatus() == "unpaid") count++;
    }
    return count;
}

double daysBetweenDates(const MyString& earlier, const MyString& later) {
    tm t1 = {};
    tm t2 = {};
    t1.tm_mday = earlier.substr(0, 2).toInt();
    t1.tm_mon = earlier.substr(3, 2).toInt() - 1;
    t1.tm_year = earlier.substr(6, 4).toInt() - 1900;
    t1.tm_hour = 12;
    t2.tm_mday = later.substr(0, 2).toInt();
    t2.tm_mon = later.substr(3, 2).toInt() - 1;
    t2.tm_year = later.substr(6, 4).toInt() - 1900;
    t2.tm_hour = 12;
    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);
    return difftime(time2, time1) / 86400.0;
}

void renderAdminMenu(AppState* state) {
    drawCenteredText("Admin Menu", 50, FONT_SIZE_TITLE, colorTextPrimary());
    MyString welcome = "Welcome, ";
    welcome += state->admin.getName();
    drawCenteredText(welcome.c_str(), 110, FONT_SIZE_BODY, colorTextSecondary());
    drawFlashMessage(state, 40, 150);

    int btnWidth = 320;
    int btnHeight = 55;
    int gapX = 40;
    int gapY = 20;
    int cols = 2;
    int totalWidth = cols * btnWidth + gapX;
    int startX = (WINDOW_WIDTH - totalWidth) / 2;
    int startY = 200;

    const char* labels[10] = {
        "Add Doctor", "Remove Doctor", "View All Patients", "View All Doctors",
        "View All Appointments", "View Unpaid Bills", "Discharge Patient",
        "Security Log", "Daily Report", "Logout"
    };
    int targets[10] = {
        SCREEN_ADMIN_ADD_DOCTOR, SCREEN_ADMIN_REMOVE_DOCTOR, SCREEN_ADMIN_ALL_PATIENTS, SCREEN_ADMIN_ALL_DOCTORS,
        SCREEN_ADMIN_ALL_APPTS, SCREEN_ADMIN_UNPAID_BILLS, SCREEN_ADMIN_DISCHARGE,
        SCREEN_ADMIN_SECURITY_LOG, SCREEN_ADMIN_DAILY_REPORT, SCREEN_ROLE_SELECT
    };

    for (int i = 0; i < 10; i++) {
        int row = i / cols;
        int col = i % cols;
        Rectangle rect = Rectangle{ (float)(startX + col * (btnWidth + gapX)), (float)(startY + row * (btnHeight + gapY)), (float)btnWidth, (float)btnHeight };
        if (drawButton(rect, labels[i])) {
            if (i == 9) {
                state->currentScreen = SCREEN_ROLE_SELECT;
            } else {
                resetAllAdminScreenState(state);
                state->currentScreen = targets[i];
            }
        }
    }
}

void renderAdminAddDoctor(AppState* state) {
    drawBackButton(state, SCREEN_ADMIN_MENU);
    drawCenteredText("Add Doctor", 50, FONT_SIZE_TITLE, colorTextPrimary());
    drawSuccessMessage(state, 140, 105);

    int leftX = 100;
    int fieldWidth = 400;
    int y = 140;

    drawFieldLabel("Name (max 50 chars)", leftX, y);
    Rectangle nameRect = Rectangle{ (float)leftX, (float)(y + 25), (float)fieldWidth, 45 };
    updateTextField(nameRect, &state->addDocNameInput, &state->addDocNameActive, 50);
    drawTextField(nameRect, state->addDocNameInput, state->addDocNameActive, false, state->cursorTimer);

    y += 90;
    drawFieldLabel("Specialization (max 50 chars)", leftX, y);
    Rectangle specRect = Rectangle{ (float)leftX, (float)(y + 25), (float)fieldWidth, 45 };
    updateTextField(specRect, &state->addDocSpecInput, &state->addDocSpecActive, 50);
    drawTextField(specRect, state->addDocSpecInput, state->addDocSpecActive, false, state->cursorTimer);

    y += 90;
    drawFieldLabel("Contact (11 digits)", leftX, y);
    Rectangle contactRect = Rectangle{ (float)leftX, (float)(y + 25), (float)fieldWidth, 45 };
    updateTextField(contactRect, &state->addDocContactInput, &state->addDocContactActive, 11);
    drawTextField(contactRect, state->addDocContactInput, state->addDocContactActive, false, state->cursorTimer);

    y += 90;
    drawFieldLabel("Password (min 6 chars)", leftX, y);
    Rectangle passRect = Rectangle{ (float)leftX, (float)(y + 25), (float)fieldWidth, 45 };
    updateTextField(passRect, &state->addDocPasswordInput, &state->addDocPasswordActive, 30);
    drawTextField(passRect, state->addDocPasswordInput, state->addDocPasswordActive, false, state->cursorTimer);

    y += 90;
    drawFieldLabel("Consultation Fee", leftX, y);
    Rectangle feeRect = Rectangle{ (float)leftX, (float)(y + 25), (float)fieldWidth, 45 };
    updateTextField(feeRect, &state->addDocFeeInput, &state->addDocFeeActive, 12);
    drawTextField(feeRect, state->addDocFeeInput, state->addDocFeeActive, false, state->cursorTimer);

    drawErrorText(state->addDocError, leftX, y + 70);

    Rectangle btn = Rectangle{ (float)leftX, (float)(y + 110), 220, 45 };
    if (drawButton(btn, "Add Doctor")) {
        attemptAddDoctor(state);
    }
}

void renderAdminRemoveDoctor(AppState* state) {
    drawBackButton(state, SCREEN_ADMIN_MENU);
    drawCenteredText("Remove Doctor", 50, FONT_SIZE_TITLE, colorTextPrimary());
    drawSuccessMessage(state, 140, 105);

    int leftX = 100;
    int tableY = 150;
    if (state->doctors.size() == 0) {
        DrawText("No doctors found.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
    } else {
        int colWidths[4] = { 80, 280, 240, 140 };
        MyString headers[4] = { MyString("ID"), MyString("Name"), MyString("Specialization"), MyString("Fee") };
        drawTableHeader(leftX, tableY, colWidths, headers, 4);
        for (int i = 0; i < state->doctors.size(); i++) {
            Doctor& d = state->doctors[i];
            MyString cells[4] = { intToMyString(d.getId()), d.getName(), d.getSpecialization(), floatToMyString(d.getFee()) };
            drawTableRow(leftX, tableY + 36 * (i + 1), colWidths, cells, 4, i % 2 == 1);
        }
    }

    int formY = tableY + 36 * (state->doctors.size() + 1) + 50;
    drawFieldLabel("Doctor ID to remove", leftX, formY);
    Rectangle idRect = Rectangle{ (float)leftX, (float)(formY + 25), 300, 45 };
    updateTextField(idRect, &state->removeDocIdInput, &state->removeDocIdActive, 10);
    drawTextField(idRect, state->removeDocIdInput, state->removeDocIdActive, false, state->cursorTimer);
    drawErrorText(state->removeDocError, leftX, formY + 80);

    Rectangle btn = Rectangle{ (float)leftX, (float)(formY + 120), 220, 45 };
    if (drawButton(btn, "Remove Doctor")) {
        attemptRemoveDoctor(state);
    }
}

void renderAdminAllPatients(AppState* state) {
    drawBackButton(state, SCREEN_ADMIN_MENU);
    drawCenteredText("All Patients", 50, FONT_SIZE_TITLE, colorTextPrimary());

    int leftX = 60;
    int tableY = 130;
    if (state->patients.size() == 0) {
        DrawText("No patients found.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
        return;
    }

    int visibleHeight = WINDOW_HEIGHT - tableY - 40;
    int maxScroll = computeMaxScroll(state->patients.size(), visibleHeight - 36, 36);
    updateScroll(state, maxScroll);

    int colWidths[7] = { 60, 220, 80, 100, 180, 140, 160 };
    MyString headers[7] = { MyString("ID"), MyString("Name"), MyString("Age"), MyString("Gender"), MyString("Contact"), MyString("Balance"), MyString("Unpaid Bills") };
    drawTableHeader(leftX, tableY, colWidths, headers, 7);

    BeginScissorMode(leftX, tableY + 36, 1160, visibleHeight - 36);
    for (int i = 0; i < state->patients.size(); i++) {
        int rowY = tableY + 36 + i * 36 - state->scrollOffset;
        if (rowY < tableY + 36 - 36 || rowY > WINDOW_HEIGHT) continue;
        Patient& p = state->patients[i];
        MyString cells[7] = { intToMyString(p.getId()), p.getName(), intToMyString(p.getAge()), p.getGender(), p.getContact(), floatToMyString(p.getBalance()), intToMyString(countUnpaidBills(state, p.getId())) };
        drawTableRow(leftX, rowY, colWidths, cells, 7, i % 2 == 1);
    }
    EndScissorMode();
}

void renderAdminAllDoctors(AppState* state) {
    drawBackButton(state, SCREEN_ADMIN_MENU);
    drawCenteredText("All Doctors", 50, FONT_SIZE_TITLE, colorTextPrimary());

    int leftX = 100;
    int tableY = 130;
    if (state->doctors.size() == 0) {
        DrawText("No doctors found.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
        return;
    }

    int colWidths[5] = { 80, 260, 240, 200, 140 };
    MyString headers[5] = { MyString("ID"), MyString("Name"), MyString("Specialization"), MyString("Contact"), MyString("Fee") };
    drawTableHeader(leftX, tableY, colWidths, headers, 5);
    for (int i = 0; i < state->doctors.size(); i++) {
        Doctor& d = state->doctors[i];
        MyString cells[5] = { intToMyString(d.getId()), d.getName(), d.getSpecialization(), d.getContact(), floatToMyString(d.getFee()) };
        drawTableRow(leftX, tableY + 36 * (i + 1), colWidths, cells, 5, i % 2 == 1);
    }
}

void renderAdminAllAppts(AppState* state) {
    drawBackButton(state, SCREEN_ADMIN_MENU);
    drawCenteredText("All Appointments", 50, FONT_SIZE_TITLE, colorTextPrimary());

    MyVector<Appointment> list = state->appointments;
    sortAppointmentsByDateDesc(&list);

    int leftX = 60;
    int tableY = 130;
    if (list.size() == 0) {
        DrawText("No appointments found.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
        return;
    }

    int visibleHeight = WINDOW_HEIGHT - tableY - 40;
    int maxScroll = computeMaxScroll(list.size(), visibleHeight - 36, 36);
    updateScroll(state, maxScroll);

    int colWidths[6] = { 80, 240, 240, 140, 120, 140 };
    MyString headers[6] = { MyString("ID"), MyString("Patient"), MyString("Doctor"), MyString("Date"), MyString("Time"), MyString("Status") };
    drawTableHeader(leftX, tableY, colWidths, headers, 6);

    BeginScissorMode(leftX, tableY + 36, 1160, visibleHeight - 36);
    for (int i = 0; i < list.size(); i++) {
        int rowY = tableY + 36 + i * 36 - state->scrollOffset;
        if (rowY < tableY + 36 - 36 || rowY > WINDOW_HEIGHT) continue;
        Appointment& a = list[i];
        MyString cells[6] = { intToMyString(a.getAppointmentId()), getPatientName(state, a.getPatientId()), getDoctorName(state, a.getDoctorId()), a.getDate(), a.getTimeSlot(), a.getStatus() };
        drawTableRow(leftX, rowY, colWidths, cells, 6, i % 2 == 1);
    }
    EndScissorMode();
}

void renderAdminUnpaidBills(AppState* state) {
    drawBackButton(state, SCREEN_ADMIN_MENU);
    drawCenteredText("Unpaid Bills", 50, FONT_SIZE_TITLE, colorTextPrimary());

    MyVector<Bill> list;
    for (int i = 0; i < state->bills.size(); i++) {
        if (state->bills[i].getStatus() == "unpaid") list.add(state->bills[i]);
    }

    int leftX = 100;
    int tableY = 130;
    if (list.size() == 0) {
        DrawText("No unpaid bills.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
        return;
    }

    MyString today = getCurrentDateString();
    int colWidths[4] = { 100, 280, 160, 220 };
    MyString headers[4] = { MyString("Bill ID"), MyString("Patient"), MyString("Amount"), MyString("Date") };
    drawTableHeader(leftX, tableY, colWidths, headers, 4);
    for (int i = 0; i < list.size(); i++) {
        Bill& b = list[i];
        MyString dateCell = b.getDate();
        if (daysBetweenDates(b.getDate(), today) > 7.0) {
            dateCell += " [OVERDUE]";
        }
        MyString cells[4] = { intToMyString(b.getBillId()), getPatientName(state, b.getPatientId()), floatToMyString(b.getAmount()), dateCell };
        drawTableRow(leftX, tableY + 36 * (i + 1), colWidths, cells, 4, i % 2 == 1);
    }
}

void renderAdminDischarge(AppState* state) {
    drawBackButton(state, SCREEN_ADMIN_MENU);
    drawCenteredText("Discharge Patient", 50, FONT_SIZE_TITLE, colorTextPrimary());
    drawSuccessMessage(state, 140, 105);

    int leftX = 100;
    int formY = 150;
    drawFieldLabel("Patient ID", leftX, formY);
    Rectangle idRect = Rectangle{ (float)leftX, (float)(formY + 25), 300, 45 };
    updateTextField(idRect, &state->dischargeIdInput, &state->dischargeIdActive, 10);
    drawTextField(idRect, state->dischargeIdInput, state->dischargeIdActive, false, state->cursorTimer);
    drawErrorText(state->dischargeError, leftX, formY + 80);

    Rectangle btn = Rectangle{ (float)leftX, (float)(formY + 120), 220, 45 };
    if (drawButton(btn, "Discharge Patient")) {
        attemptDischarge(state);
    }
}

void renderAdminSecurityLog(AppState* state) {
    drawBackButton(state, SCREEN_ADMIN_MENU);
    drawCenteredText("Security Log", 50, FONT_SIZE_TITLE, colorTextPrimary());

    MyVector<MyString> lines;
    state->fileHandler.readAllLines("security_log.txt", lines);

    int leftX = 100;
    int tableY = 130;
    if (lines.size() <= 1) {
        DrawText("No security events logged.", leftX, tableY, FONT_SIZE_BODY, colorTextSecondary());
        return;
    }

    int visibleHeight = WINDOW_HEIGHT - tableY - 40;
    int rowCount = lines.size() - 1;
    int maxScroll = computeMaxScroll(rowCount, visibleHeight, 36);
    updateScroll(state, maxScroll);

    BeginScissorMode(leftX, tableY, 1080, visibleHeight);
    for (int i = 1; i < lines.size(); i++) {
        int rowY = tableY + (i - 1) * 36 - state->scrollOffset;
        if (rowY + 36 < tableY || rowY > WINDOW_HEIGHT) continue;
        DrawRectangle(leftX, rowY, 1080, 36, (i % 2 == 0) ? colorPanelAlt() : colorPanel());
        DrawText(lines[i].c_str(), leftX + 8, rowY + 8, FONT_SIZE_BODY, colorTextPrimary());
    }
    EndScissorMode();
}

void renderAdminDailyReport(AppState* state) {
    drawBackButton(state, SCREEN_ADMIN_MENU);
    drawCenteredText("Daily Report", 50, FONT_SIZE_TITLE, colorTextPrimary());

    MyString today = getCurrentDateString();
    int totalCount = 0, pendingCount = 0, completedCount = 0, noshowCount = 0, cancelledCount = 0;
    for (int i = 0; i < state->appointments.size(); i++) {
        Appointment& a = state->appointments[i];
        if (a.getDate() == today) {
            totalCount++;
            if (a.getStatus() == "pending") pendingCount++;
            else if (a.getStatus() == "completed") completedCount++;
            else if (a.getStatus() == "noshow") noshowCount++;
            else if (a.getStatus() == "cancelled") cancelledCount++;
        }
    }

    int leftX = 80;
    int y = 120;
    MyString header = "Appointments Today -- Total: ";
    header += intToMyString(totalCount);
    header += "  Pending: ";
    header += intToMyString(pendingCount);
    header += "  Completed: ";
    header += intToMyString(completedCount);
    header += "  No-show: ";
    header += intToMyString(noshowCount);
    header += "  Cancelled: ";
    header += intToMyString(cancelledCount);
    DrawText(header.c_str(), leftX, y, FONT_SIZE_BODY, colorTextPrimary());

    float revenue = 0.0f;
    for (int i = 0; i < state->bills.size(); i++) {
        if (state->bills[i].getStatus() == "paid" && state->bills[i].getDate() == today) {
            revenue += state->bills[i].getAmount();
        }
    }
    MyString revText = "Revenue Collected Today: PKR ";
    revText += floatToMyString(revenue);
    DrawText(revText.c_str(), leftX, y + 36, FONT_SIZE_BODY, colorTextPrimary());

    y += 90;
    DrawText("Patients with Outstanding Unpaid Bills", leftX, y, FONT_SIZE_HEADING, colorTextPrimary());
    y += 40;

    MyVector<int> ownerIds;
    MyVector<float> ownerAmounts;
    for (int i = 0; i < state->bills.size(); i++) {
        if (state->bills[i].getStatus() == "unpaid") {
            int pid = state->bills[i].getPatientId();
            int idx = -1;
            for (int j = 0; j < ownerIds.size(); j++) {
                if (ownerIds[j] == pid) {
                    idx = j;
                    break;
                }
            }
            if (idx == -1) {
                ownerIds.add(pid);
                ownerAmounts.add(state->bills[i].getAmount());
            } else {
                ownerAmounts[idx] += state->bills[i].getAmount();
            }
        }
    }

    if (ownerIds.size() == 0) {
        DrawText("None", leftX, y, FONT_SIZE_BODY, colorTextSecondary());
        y += 36;
    } else {
        int colWidths[2] = { 320, 200 };
        MyString headers[2] = { MyString("Patient Name"), MyString("Total Owed") };
        drawTableHeader(leftX, y, colWidths, headers, 2);
        for (int i = 0; i < ownerIds.size(); i++) {
            MyString cells[2] = { getPatientName(state, ownerIds[i]), floatToMyString(ownerAmounts[i]) };
            drawTableRow(leftX, y + 36 * (i + 1), colWidths, cells, 2, i % 2 == 1);
        }
        y += 36 * (ownerIds.size() + 1);
    }

    y += 40;
    DrawText("Doctor-wise Summary for Today", leftX, y, FONT_SIZE_HEADING, colorTextPrimary());
    y += 40;

    int colWidths2[4] = { 280, 140, 140, 140 };
    MyString headers2[4] = { MyString("Doctor"), MyString("Completed"), MyString("Pending"), MyString("No-show") };
    drawTableHeader(leftX, y, colWidths2, headers2, 4);
    for (int i = 0; i < state->doctors.size(); i++) {
        int comp = 0, pend = 0, noshow = 0;
        for (int j = 0; j < state->appointments.size(); j++) {
            Appointment& a = state->appointments[j];
            if (a.getDoctorId() == state->doctors[i].getId() && a.getDate() == today) {
                if (a.getStatus() == "completed") comp++;
                else if (a.getStatus() == "pending") pend++;
                else if (a.getStatus() == "noshow") noshow++;
            }
        }
        MyString cells[4] = { state->doctors[i].getName(), intToMyString(comp), intToMyString(pend), intToMyString(noshow) };
        drawTableRow(leftX, y + 36 * (i + 1), colWidths2, cells, 4, i % 2 == 1);
    }
}

void updateGlobalTimers(AppState* state) {
    state->cursorTimer++;
    updateSuccessTimer(state);
    updateFlashTimer(state);
}

void renderCurrentScreen(AppState* state) {
    switch (state->currentScreen) {
        case SCREEN_ROLE_SELECT: renderRoleSelect(state); break;
        case SCREEN_LOGIN: renderLogin(state); break;
        case SCREEN_LOCKED: renderLocked(state); break;
        case SCREEN_PATIENT_MENU: renderPatientMenu(state); break;
        case SCREEN_PATIENT_BOOK: renderPatientBook(state); break;
        case SCREEN_PATIENT_CANCEL: renderPatientCancel(state); break;
        case SCREEN_PATIENT_VIEW_APPTS: renderPatientViewAppts(state); break;
        case SCREEN_PATIENT_VIEW_RECORDS: renderPatientViewRecords(state); break;
        case SCREEN_PATIENT_VIEW_BILLS: renderPatientViewBills(state); break;
        case SCREEN_PATIENT_PAY_BILL: renderPatientPayBill(state); break;
        case SCREEN_PATIENT_TOPUP: renderPatientTopup(state); break;
        case SCREEN_DOCTOR_MENU: renderDoctorMenu(state); break;
        case SCREEN_DOCTOR_TODAY: renderDoctorToday(state); break;
        case SCREEN_DOCTOR_COMPLETE: renderDoctorComplete(state); break;
        case SCREEN_DOCTOR_NOSHOW: renderDoctorNoshow(state); break;
        case SCREEN_DOCTOR_PRESCRIBE: renderDoctorPrescribe(state); break;
        case SCREEN_DOCTOR_HISTORY: renderDoctorHistory(state); break;
        case SCREEN_ADMIN_MENU: renderAdminMenu(state); break;
        case SCREEN_ADMIN_ADD_DOCTOR: renderAdminAddDoctor(state); break;
        case SCREEN_ADMIN_REMOVE_DOCTOR: renderAdminRemoveDoctor(state); break;
        case SCREEN_ADMIN_ALL_PATIENTS: renderAdminAllPatients(state); break;
        case SCREEN_ADMIN_ALL_DOCTORS: renderAdminAllDoctors(state); break;
        case SCREEN_ADMIN_ALL_APPTS: renderAdminAllAppts(state); break;
        case SCREEN_ADMIN_UNPAID_BILLS: renderAdminUnpaidBills(state); break;
        case SCREEN_ADMIN_DISCHARGE: renderAdminDischarge(state); break;
        case SCREEN_ADMIN_SECURITY_LOG: renderAdminSecurityLog(state); break;
        case SCREEN_ADMIN_DAILY_REPORT: renderAdminDailyReport(state); break;
        default: break;
    }
}

void loadAllData(AppState* state) {
    try {
        state->fileHandler.loadPatients(state->patients);
        state->fileHandler.loadDoctors(state->doctors);
        state->fileHandler.loadAdmin(state->admin);
        state->fileHandler.loadAppointments(state->appointments);
        state->fileHandler.loadPrescriptions(state->prescriptions);
        state->fileHandler.loadBills(state->bills);
    } catch (FileNotFoundException& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
    }
}

void runMainLoop(AppState* state) {
    while (!WindowShouldClose()) {
        updateGlobalTimers(state);
        BeginDrawing();
        ClearBackground(colorBackground());
        renderCurrentScreen(state);
        EndDrawing();
    }
}

int main() {
    AppState* state = new AppState();
    loadAllData(state);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MediCore Hospital Management System");
    SetTargetFPS(60);

    runMainLoop(state);

    CloseWindow();
    delete state;
    return 0;
}

