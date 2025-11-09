#ifndef PATIENT_ADMISSION_CLERK_H
#define PATIENT_ADMISSION_CLERK_H

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "queue.hpp"
#include "patient_array.hpp"
#include "file_handling.hpp"
// #include "../utils/message_handler.hpp"

class PatientAdmissionClerk {
    private:
        std::string name;
        Queue patientQueue;
        PatientArray allPatients;
    public:
        PatientAdmissionClerk();
        void displayMenu();
        void admitNewPatient();
        void admitExistingPatient();
        void viewPatientQueue();
        void viewPatient();
        void dischargePatient();
};

#endif