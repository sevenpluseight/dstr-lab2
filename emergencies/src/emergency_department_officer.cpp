
#include "emergency_department_officer.hpp"
#include "string_utils.hpp"
#include "message_handler.hpp"
#include "path_utils.hpp"
#include "time_utils.hpp"

EmergencyDepartmentOfficer::EmergencyDepartmentOfficer() {
    dataFile = getDataFilePath("emergency_cases.csv");
    manager.loadFromCSV(dataFile);
}

void EmergencyDepartmentOfficer::run() {
    displayMenu();
}

// Main menu
void EmergencyDepartmentOfficer::displayMenu() {
    while (true) {
        std::cout << "\n--- Emergency Department Officer Menu ---\n";
        std::cout << "1. View all emergency cases\n";
        std::cout << "2. Add new emergency case\n";
        std::cout << "3. Process highest priority case\n";
        std::cout << "4. Exit\n";
        std::cout << "Select an option: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") viewCases();
        else if (choice == "2") addCase();
        else if (choice == "3") processHighestPriorityCase();
        else if (choice == "4") {
            manager.saveToCSV(dataFile);
            MessageHandler::info("Exiting Emergency Department Officer menu...");
            break;
        } else {
            MessageHandler::warning("Invalid option. Please try again.");
        }
    }
}

// View cases
void EmergencyDepartmentOfficer::viewCases() {
    manager.printAllCases();
}

// Add new case
void EmergencyDepartmentOfficer::addCase() {
    EmergencyCase ec;
    // Auto-generate Case ID
    ec.case_id = manager.generateNextCaseID();
    std::cout << "\nGenerated Case ID: " << ec.case_id << "\n";
    ec.patient_id = getValidatedInput("Enter Patient ID: ", [](const std::string &s){ return !s.empty(); }, "Patient ID cannot be empty.");
    ec.patient_name = getValidatedInput("Enter Patient Name: ", [](const std::string &s){ return !s.empty(); }, "Patient name cannot be empty.");
    ec.emergency_type = getValidatedInput("Enter Emergency Type: ", [](const std::string &s){ return !s.empty(); }, "Emergency type cannot be empty.");
    
    std::string prio = getValidatedInput("Enter Priority Level (1-5): ", [](const std::string &s){
        return s.size() == 1 && s[0] >= '1' && s[0] <= '5';
    }, "Priority must be 1-5.");
    ec.priority_level = prio[0] - '0';
    
    ec.status = "Pending";
    ec.timestamp_logged = getCurrentTimestamp();
    ec.timestamp_processed = "";
    ec.ambulance_id = "";

    manager.addCase(ec);
    manager.saveToCSV(dataFile);
    MessageHandler::info("Emergency case added successfully.");
}

// Process highest priority case
void EmergencyDepartmentOfficer::processHighestPriorityCase() {
    if (manager.isEmpty()) {
        MessageHandler::warning("No emergency cases to process.");
        return;
    }

    EmergencyCase ec = manager.popHighestPriorityCase();
    std::cout << "\nProcessing Case: " << ec.case_id << " | " << ec.patient_name << "\n";

    // Here we can assign ambulance or update status
    ec.status = "Completed";
    ec.timestamp_processed = getCurrentTimestamp();

    manager.updateCase(ec);  // Re-insert to maintain linked list if needed
    manager.saveToCSV(dataFile);

    MessageHandler::info("Case processed successfully.");
}
