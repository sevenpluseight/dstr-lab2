#include "emergency_department_officer.hpp"
#include "string_utils.hpp" 
#include "message_handler.hpp"
#include "path_utils.hpp"
#include "time_utils.hpp"
#include <cstdlib> // For rand()
#include <string>
#include <ctime>
#include <cctype>
#include <limits>

// Helper function to check for digits in a string
bool containsDigits(const std::string& s) {
    for (size_t i = 0; i < s.length(); ++i) {
        if (isdigit(s[i])) {
            return true; // Found a digit
        }
    }
    return false; // No digits found
}

// Helper function to safely read an integer
int getValidatedInt(const std::string& prompt, int min, int max) {
    int choice = 0;
    std::string input;
    while (true) {
        std::cout << prompt; // <-- FIX #1: Prints the prompt every time
        
        if (!std::getline(std::cin, input)) {
            // --- FIX #2: Handle ^Z (EOF) ---
            if (std::cin.eof()) {
                MessageHandler::error("\nEnd-of-File detected. Exiting program.");
                exit(1); // Exit gracefully
            }
            // Handle other unknown stream errors
            std::cin.clear(); // Clear the error state
            // Discard the rest of the bad line
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            MessageHandler::warning("An input error occurred. Please try again.");
            continue;
            // --- END FIX #2 ---
        }

        trim(input);

        if (input.empty()) {
            MessageHandler::warning("Input cannot be empty. Please enter a number.");
            continue; // Loop again (will re-print prompt)
        }

        try {
            choice = std::stoi(input);
            if (choice >= min && choice <= max) {
                return choice; // Valid choice
            } else {
                MessageHandler::warning("Please enter a number between " + std::to_string(min) + " and " + std::to_string(max) + ".");
            }
        } catch (const std::invalid_argument&) {
            MessageHandler::warning("Invalid input. Please enter a number.");
        }
    }
}

EmergencyDepartmentOfficer::EmergencyDepartmentOfficer() {
    // Seed the random number generator *once*
    srand(static_cast<unsigned int>(time(NULL)));

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
    ec.case_id = manager.generateNextCaseID();
    std::cout << "\nGenerated Case ID: " << ec.case_id << "\n";
    
// --- Patient ID Input (Now with Full Validation) ---
    while (true) {
        std::cout << "Enter Patient ID (e.g., PAT-0001): ";
        std::getline(std::cin, ec.patient_id);
        trim(ec.patient_id);

        if (ec.patient_id.empty()) {
            MessageHandler::warning("Patient ID cannot be empty.");
            continue; // Ask again
        }

        // 1. New Validation: Check format (must start with "PAT-")
        // rfind() returns 0 if the string starts with the prefix
        if (ec.patient_id.rfind("PAT-", 0) != 0) {
            MessageHandler::error("Invalid Format. Patient ID must start with 'PAT-'.");
            continue; // Ask again
        }

        // 2. New Validation: Check if ID exists in the patient list
        ec.patient_name = manager.getPatientName(ec.patient_id);
        
        if (ec.patient_name == "Unknown") {
            MessageHandler::error("Patient ID not found in database. Please try again.");
            // We do *not* ask for the name. We force a valid ID.
        } else {
            // Success! The ID is valid and we found the name.
            MessageHandler::info("Found Patient: " + ec.patient_name);
            break; // Exit the loop and proceed
        }
    }

    // --- NEW: Emergency Type Selection Menu ---
    std::cout << "Select Emergency Type:\n";
    int typeCount = manager.printAllTypes(); // Prints the list
    int otherOption = typeCount + 1;
    std::cout << otherOption << ". Other (Enter a new emergency type)\n";

    // Create the prompt string to pass to our new function
    std::string typePrompt = "Select an option (1-" + std::to_string(otherOption) + "): ";

    // Call the new function with the prompt
    int choice = getValidatedInt(typePrompt, 1, otherOption);
    
        if (choice == otherOption) {
        // User chose "Other", ask for manual input
        while (true) {
            std::cout << "Enter a new emergency type: ";
            
            // Read directly into the emergency_type string
            if (!std::getline(std::cin, ec.emergency_type)) {
                if (std::cin.eof()) {
                    MessageHandler::error("\nEnd-of-File detected. Exiting program.");
                    exit(1);
                }
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                MessageHandler::warning("An input error occurred. Please try again.");
                continue;
            }

            trim(ec.emergency_type);

            if (ec.emergency_type.empty()) {
                MessageHandler::warning("Emergency type cannot be empty.");
            } else if (containsDigits(ec.emergency_type)) {
                MessageHandler::warning("Emergency Type cannot contain numbers.");
            } else {
                if (manager.typeExists(toUpper(ec.emergency_type))) {
                    MessageHandler::info("Note: This emergency type already exists and will be used.");
                }
                break; // Good input
            }
        }
    } else {
        // User picked an existing type from the list
        ec.emergency_type = manager.getTypeByIndex(choice);
        MessageHandler::info("Selected type: " + ec.emergency_type);
    }
    
    // --- Priority Level Input (Unchanged) ---
    std::string prio;
    while (true) {
        std::cout << "Enter Priority Level (1-5): ";
        std::getline(std::cin, prio);
        trim(prio);
        if (prio.size() == 1 && prio[0] >= '1' && prio[0] <= '5') {
            break;
        }
        MessageHandler::warning("Priority must be a single number from 1 to 5.");
    }
    ec.priority_level = prio[0] - '0';
    
    // --- Final steps (Unchanged) ---
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