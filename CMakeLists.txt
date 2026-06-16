cmake_minimum_required(VERSION 3.15)
project(MediCore CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
)
FetchContent_MakeAvailable(raylib)

add_executable(MediCore
    main.cpp
    MyString.cpp
    Person.cpp
    Patient.cpp
    Doctor.cpp
    Admin.cpp
    Appointment.cpp
    Bill.cpp
    Prescription.cpp
    FileHandler.cpp
    Validator.cpp
    HospitalException.cpp
    FileNotFoundException.cpp
    InsufficientFundsException.cpp
    InvalidInputException.cpp
    SlotUnavailableException.cpp
)

target_link_libraries(MediCore raylib)

set(MEDICORE_DATA_FILES
    patients.txt
    doctors.txt
    admin.txt
    appointments.txt
    prescriptions.txt
    bills.txt
    security_log.txt
    discharged.txt
)

foreach(dataFile ${MEDICORE_DATA_FILES})
    add_custom_command(TARGET MediCore POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/${dataFile}
            $<TARGET_FILE_DIR:MediCore>/${dataFile}
    )
endforeach()
