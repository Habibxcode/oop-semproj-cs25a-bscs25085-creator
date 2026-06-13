# MediCore Hospital Management System

MediCore is a desktop Hospital Management System built in C++17 with a custom
Raylib graphical interface. It supports three roles -- Patient, Doctor, and
Admin -- and covers appointment booking, billing, prescriptions, doctor
management, and administrative reporting.

The project intentionally avoids the C++ Standard Library containers and
string classes (no `std::vector`, `std::string`, `<algorithm>`, `<sstream>`,
etc.). All dynamic collections and strings are implemented from scratch
(`MyVector`, `MyString`), and all file I/O goes through a single
`FileHandler` class using manual CSV parsing.

## Features

- **Patients**: book and cancel appointments, view appointment history and
  medical records, view and pay bills, top up account balance.
- **Doctors**: view today's appointments, mark appointments complete or
  no-show, write prescriptions, view patient medical history.
- **Admin**: manage doctors (add/remove), view all patients/doctors/
  appointments, view unpaid bills, discharge patients, view the security
  log, and generate a daily report.
- Login lockout after 3 failed attempts, with all login attempts recorded in
  `security_log.txt`.

## Requirements

- CMake 3.15 or later
- A C++17 compiler (MSVC, MinGW/GCC, or Clang)
- Internet access for the first build (CMake's `FetchContent` downloads
  raylib 5.0 automatically)

## Building

```sh
cmake -S . -B build
cmake --build build --config Release
```

The data files (`patients.txt`, `doctors.txt`, `admin.txt`,
`appointments.txt`, `prescriptions.txt`, `bills.txt`, `security_log.txt`,
`discharged.txt`) are copied next to the built executable automatically as a
post-build step.

## Running

After building, run the `MediCore` executable from the directory that
contains the copied `.txt` data files (the build output directory):

```sh
./build/Release/MediCore     # or build/MediCore, depending on generator
```

A window titled "MediCore Hospital Management System" (1280x800) will open
on the role selection screen.

## Default Credentials

### Patients

| ID | Password |
|----|----------|
| 1  | pass123  |
| 2  | sara456  |
| 3  | bil789   |

### Doctors

| ID | Password |
|----|----------|
| 1  | doc456   |
| 2  | doc789   |
| 3  | doc321   |

### Admin

| ID | Password |
|----|----------|
| 1  | admin123 |

## Repository

[YOUR_GITHUB_LINK_HERE]
