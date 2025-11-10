#include "patient_admission_clerk.hpp"
#include "path_utils.hpp"
#include "message_handler.hpp"

PatientAdmissionClerk::PatientAdmissionClerk() {
    name = "";
    patientFilepath = getDataFilePath("patient_data.csv");
    queueFilePath = getDataFilePath("queue.csv");
    allPatients = readPatientFile(allPatients, patientFilepath);
    patientQueue = readQueueFile(patientQueue, queueFilePath);
}

void PatientAdmissionClerk::displayMenu() {

    int choice;

    do {
        std::cout << "\n-------------- PATIENT ADMISSION MANAGEMENT MENU --------------"  << std::endl;
        std::cout << "1. Admit New Patient"                                             << std::endl;
        std::cout << "2. Admit Existing Patient"                                        << std::endl;
        std::cout << "3. View Patient Queue"                                            << std::endl;
        std::cout << "4. View Patient Latest Condition"                                 << std::endl;
        std::cout << "5. Discharge Patient"                                             << std::endl;
        std::cout << "6. Exit"                                                          << std::endl;
        std::cout << "---------------------------------------------------------------"  << std::endl;
        std::cout << "Enter a number to perform an action: " << std::flush;

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            MessageHandler::error("Invalid input. Please enter a number.");
            continue;
        }
        std::cin.ignore();

        switch (choice) {
            case 1:
                admitNewPatient();
                break;
            case 2:
                admitExistingPatient();
                break;
            case 3:
                viewPatientQueue();
                break;
            case 4:
                viewPatient();
                break;
            case 5:
                dischargePatient();
                break;
            case 6:
                MessageHandler::info("Exiting Patient Admission System...\n");
                break;
            default:
                MessageHandler::error("Invalid choice. Please select a valid option.");
                break;
        }
    } while (choice != 6);
}

void PatientAdmissionClerk::admitNewPatient() {

    Patient patient;

    std::cout << "\n------------------ NEW PATIENT ADMISSION FORM ------------------"  << std::endl;
    std::cout << "Patient Name: " << std::flush;
    std::getline(std::cin, patient.name);
    std::cout << "Patient Age: " << std::flush;
    std::getline(std::cin, patient.age);
    std::cout << "Patient Blood Type (e.g. A+): " << std::flush;
    std::getline(std::cin, patient.bloodType);
    std::cout << "Patient Height (cm, e.g. 180.50): " << std::flush;
    std::getline(std::cin, patient.height);
    std::cout << "Patient Weight (kg, e.g. 60.89): " << std::flush;
    std::getline(std::cin, patient.weight);
    std::cout << "Patient Temperature (C, e.g. 36.15): " << std::flush;
    std::getline(std::cin, patient.temperature);
    std::cout << "Patient Heart Rate (bpm, e.g. 75): " << std::flush;
    std::getline(std::cin, patient.heartRate);
    std::cout << "Patient Blood Pressure (mmHg, e.g. 113/76): " << std::flush;
    std::getline(std::cin, patient.bloodPressure);
    std::cout << "Patient Condition: " << std::flush;
    std::getline(std::cin, patient.condition);
    std::cout << "---------------------------------------------------------------"  << std::endl;

    const double result = std::stod(patient.weight)/((std::stod(patient.height)/100)*(std::stod(patient.height)/100));
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << result;
    patient.BMI = stream.str();

    std::string newID = std::to_string(allPatients.getSize() + 1);
    newID.insert(0, 4 - newID.length(), '0');
    newID = "PAT-" + newID;
    patient.patient_id = newID;

    allPatients.appendArray(patient);
    appendPatientFile(patient.getLine(), patientFilepath);

    patientQueue.enqueue(patient.patient_id, patient.condition);
    enqueueQueueFile(newID, patient.condition, queueFilePath);
}

void PatientAdmissionClerk::admitExistingPatient() {

    std::string ID, Condition;

    std::cout << "\n--------------- EXISTING PATIENT ADMISSION FORM ---------------"  << std::endl;
    std::cout << "\nPatient ID: " << std::flush;
    std::getline(std::cin, ID);

    if(!allPatients.isInArray(ID)) {
        MessageHandler::info("Patient does not exist.");
        return;
    }

    if(patientQueue.inQueue(ID)) {
        MessageHandler::info("Patient already in queue...\n");
        return;
    }

    std::cout << "\nPatient Condition: " << std::endl;
    std::getline(std::cin, Condition);
    std::cout << "---------------------------------------------------------------"  << std::endl;

    Patient patient = allPatients.getPatientWithID(ID);
    patient.condition = Condition;

    modifyPatientFile(ID, patient.getLine(), patientFilepath);
    allPatients = readPatientFile(allPatients, patientFilepath);

    patientQueue.enqueue(patient.patient_id, patient.condition);
    enqueueQueueFile(ID, patient.condition, queueFilePath);
}

void PatientAdmissionClerk::viewPatientQueue() {
    int queueLength = patientQueue.getLength();
    constexpr int columnWidthOfQueueID = 8;
    constexpr int columnWidthOfPatientID = 14;
    constexpr int columnWidthOfName = 20;
    constexpr int columnWidthOfCondition = 30;
    constexpr int columnWidthOfAdmissionTime = 30;

    std::cout << std::left <<
        std::setw(columnWidthOfQueueID) << "\nQueue" <<
        std::setw(columnWidthOfPatientID) << "Patient ID" <<
        std::setw(columnWidthOfName) << "Name" <<
        std::setw(columnWidthOfCondition) << "Condition" <<
        std::setw(columnWidthOfAdmissionTime) << "Admission Time" <<
        std::endl;

    std::cout << "\n" << std::string(columnWidthOfQueueID + columnWidthOfPatientID + columnWidthOfName +
        columnWidthOfCondition + columnWidthOfAdmissionTime, '-') << "\n";

    Node* temp = patientQueue.getFront();
    for (int i = 0; i < queueLength; i++) {
        std::cout << std::left <<
            std::setw(columnWidthOfQueueID) << i + 1 <<
            std::setw(columnWidthOfPatientID) << temp->patient_id <<
            std::setw(columnWidthOfName) << allPatients.getPatientWithID(temp->patient_id).name <<
            std::setw(columnWidthOfCondition) << temp->condition <<
            std::setw(columnWidthOfAdmissionTime) << temp->admission <<
            "\n";

        temp = temp->next;
    }
    delete temp;
}

void PatientAdmissionClerk::viewPatient() {

    std::string ID;
    std::cout << "\nEnter Patient ID to search: " << std::flush;
    std::cin >> ID;

    if(!allPatients.isInArray(ID)) {
        MessageHandler::info("Patient does not exist.");;
    }
    else {
        constexpr int columnWidthOfCondition = 40;
        constexpr int columnWidthOfName = 20;
        constexpr int columnWidthOfPatientID = 14;
        MessageHandler::info("Patient found.");

        std::cout << std::left <<
            std::setw(columnWidthOfPatientID) << "\nPatient ID" <<
            std::setw(columnWidthOfName) << "Name" <<
            std::setw(columnWidthOfCondition) << "Condition" <<
            std::endl;

        std::cout << std::string(columnWidthOfPatientID + columnWidthOfName + columnWidthOfCondition, '-');

        Patient patient = allPatients.getPatientWithID(ID);
        std::cout << "\n" << std::left <<
            std::setw(columnWidthOfPatientID) << patient.patient_id <<
            std::setw(columnWidthOfName) << patient.name <<
            std::setw(columnWidthOfCondition) << patient.condition <<
            "\n" << std::endl;
    }
}

void PatientAdmissionClerk::dischargePatient() {
    const std::string ID = patientQueue.getFrontPatientID();

    bool success = patientQueue.dequeue();
    if (success) {
        dequeueQueueFile(queueFilePath);
        MessageHandler::info("Patient " + ID + " has been discharged.\n");
    }
    else {
        MessageHandler::error("Patient " + ID + " is not discharged.\n");
    }
}

    