#include "emergency_department_officer.hpp"
#include "string_utils.hpp" // Assuming getValidatedInput is in here
#include "message_handler.hpp"
#include "path_utils.hpp"
#include "time_utils.hpp"
#include <cstdlib> // For rand()

EmergencyDepartmentOfficer::EmergencyDepartmentOfficer() {
    dataFile = getDataFilePath("emergency_cases.csv");
    patientDataFile = getDataFilePath("patient_data.csv"); 
    
    // Load patient names FIRST
    manager.loadPatientData(patientDataFile);
    
    // THEN load cases (which will now use the patient names)
    manager.loadFromCSV(dataFile);
}

void EmergencyDepartmentOfficer::run() {
    displayMenu();
}

// Main menu (Unchanged)
void EmergencyDepartmentOfficer::displayMenu() {
    while (true) {
        std::cout << "\n--- Emergency Department Officer Menu ---\n";
        std::cout << "1. View emergency cases\n";
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

// View cases (Unchanged)
void EmergencyDepartmentOfficer::viewCases() {
    while (true) {
        std::cout << "\n--- View Emergency Cases ---\n";
        std::cout << "1. View Pending Cases\n";
        std::cout << "2. View Processing Cases\n";
        std::cout << "3. View Completed Cases\n";
        std::cout << "4. View All Cases\n";
        std::cout << "5. Back to Main Menu\n";
        std::cout << "Select an option: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1")
            manager.printCasesByStatus("Pending");
        else if (choice == "2")
            manager.printCasesByStatus("Processing");
        else if (choice == "3")
            manager.printCasesByStatus("Completed");
        else if (choice == "4")
            manager.printAllCases();
        else if (choice == "5")
            break;
        else
            MessageHandler::warning("Invalid option. Please try again.");
    }
}

void EmergencyDepartmentOfficer::addCase() {
    EmergencyCase ec;
    // Auto-generate Case ID
    ec.case_id = manager.generateNextCaseID();
    std::cout << "\nGenerated Case ID: " << ec.case_id << "\n";
    
    ec.patient_id = getValidatedInput("Enter Patient ID (e.g., PAT-0001): ", [](const std::string &s){ return !s.empty(); }, "Patient ID cannot be empty.");
    
    // --- Automatically look up patient name ---
    ec.patient_name = manager.getPatientName(ec.patient_id);
    if (ec.patient_name == "Unknown") {
        MessageHandler::warning("Patient ID not found. Name set to 'Unknown'.");
    } else {
        MessageHandler::info("Found Patient: " + ec.patient_name);
    }
    // --- (No longer asking user for name) ---

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

// Process highest priority case (Unchanged)
void EmergencyDepartmentOfficer::processHighestPriorityCase() {
    if (manager.isEmpty()) {
        MessageHandler::warning("No emergency cases to process.");
        return;
    }

    // Pop the highest-priority case (lowest number = most critical)
    EmergencyCase ec = manager.popHighestPriorityCase();

    std::cout << "\nProcessing Case: " << ec.case_id
              << " | " << ec.patient_name
              << " | Emergency: " << ec.emergency_type
              << " | Priority: " << ec.priority_level << "\n";

    // Automatically mark as Processing (optional step for realism)
    ec.status = "Processing";
    ec.timestamp_processed = getCurrentTimestamp(); // mark processing timestamp

    // Here we can optionally assign an ambulance
    if (ec.ambulance_id.empty()) {
        ec.ambulance_id = "AMB" + std::to_string(40 + rand() % 10);
    }

    // After processing, mark as Completed immediately (simplified simulation)
    ec.status = "Completed";

    // Re-insert or update in the list and save to CSV
    manager.updateCase(ec);
    manager.saveToCSV(dataFile);

    MessageHandler::info("Case processed and completed successfully.");
}