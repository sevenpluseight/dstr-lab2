#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "emergency_manager.hpp"
// Note: path_utils.hpp is no longer needed here, it's used in emergency_department_officer.cpp

// (The standalone getPatientNameByID function is removed)

namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[31m"; // Pending
    const std::string YELLOW  = "\033[33m"; // Processing
    const std::string GREEN   = "\033[32m"; // Completed
}

EmergencyManager::~EmergencyManager() {
    // Clear the emergency case list
    Node* current = head;
    while (current) {
        Node* tmp = current;
        current = current->next;
        delete tmp;
    }

    // Clear the patient data list
    PatientNode* pCurrent = patientHead;
    while (pCurrent) {
        PatientNode* pTmp = pCurrent;
        pCurrent = pCurrent->next;
        delete pTmp;
    }
}

// --- New functions implementation ---

// Adds a new patient to the front of the patient list
void EmergencyManager::addPatient(const std::string& id, const std::string& name) {
    PatientNode* newNode = new PatientNode;
    newNode->patientID = id;
    newNode->patientName = name;
    newNode->next = patientHead; // Add to front
    patientHead = newNode;
}

// Loads patient data into the list *once*
void EmergencyManager::loadPatientData(const std::string& patientDataFile) {
    std::ifstream file(patientDataFile);
    if (!file.is_open()) {
        MessageHandler::warning("Patient data CSV not found: " + patientDataFile);
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string patientID, patientName;
        
        std::getline(ss, patientID, ','); // Column 1: Patient_ID
        std::getline(ss, patientName, ','); // Column 2: Name
        
        if (!patientID.empty() && !patientName.empty()) {
            trim(patientID); 
            trim(patientName);
            addPatient(patientID, patientName); // Use our new function
        }
    }
    file.close();
}

// Gets a patient name from the *in-memory list*
std::string EmergencyManager::getPatientName(const std::string& patientID) const {
    PatientNode* current = patientHead;
    while (current) {
        if (current->patientID == patientID) {
            return current->patientName; // Found it
        }
        current = current->next;
    }
    return "Unknown"; // Not found
}

// --- Modified loadFromCSV ---

// Load from CSV
void EmergencyManager::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        MessageHandler::warning("Emergency CSV not found, starting empty.");
        return;
    }

    std::string line;
    std::getline(file, line); // skip header

    // Check if the header contains "patient_name"
    bool hasNameColumn = (line.find("patient_name") != std::string::npos);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        EmergencyCase ec;
        std::getline(ss, ec.case_id, ',');
        std::getline(ss, ec.patient_id, ',');

        if (hasNameColumn) {
            // If file *already* has names (from a previous save), read it
            std::getline(ss, ec.patient_name, ',');
        } else {
            // If not, look it up from our *fast in-memory list*
            ec.patient_name = getPatientName(ec.patient_id);
        }

        std::getline(ss, ec.emergency_type, ',');
        std::string prio;
        std::getline(ss, prio, ',');
        
        if (prio.empty()) prio = "5"; // Handle empty priority

        try {
            ec.priority_level = std::stoi(prio);
        } catch (const std::invalid_argument&) {
            ec.priority_level = 5; // Default priority on error
        }

        std::getline(ss, ec.status, ',');
        std::getline(ss, ec.timestamp_logged, ',');
        std::getline(ss, ec.timestamp_processed, ',');
        std::getline(ss, ec.ambulance_id, ',');

        // The old, slow call is no longer needed:
        // ec.patient_name = getPatientNameByID(ec.patient_id);

        addCase(ec); // insert in priority order
    }
    file.close();
}

// --- Unchanged Functions Below ---

// Save to CSV (This function was already correct)
void EmergencyManager::saveToCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "case_id,patient_id,patient_name,emergency_type,priority_level,status,timestamp_logged,timestamp_processed,ambulance_id\n";

    Node* current = head;
    while (current) {
        EmergencyCase& ec = current->data;
        file << ec.case_id << "," << ec.patient_id << "," << ec.patient_name << ","
             << ec.emergency_type << "," << ec.priority_level << ","
             << ec.status << "," << ec.timestamp_logged << "," 
             << ec.timestamp_processed << "," << ec.ambulance_id << "\n";
        current = current->next;
    }
    file.close();
}

// Helper function to print a single case row (This function was already correct)
void printCaseRow(const EmergencyCase& ec) {
    std::string statusColor;

    if (ec.status == "Pending") {
        statusColor = Color::RED;
    } else if (ec.status == "Processing") {
        statusColor = Color::YELLOW;
    } else if (ec.status == "Completed") {
        statusColor = Color::GREEN;
    } else {
        statusColor = Color::RESET;
    }

    std::cout << std::left
              << std::setw(13)  << ec.case_id
              << std::setw(13) << ec.patient_id
              << std::setw(19) << ec.patient_name
              << std::setw(29) << ec.emergency_type
              << std::setw(10) << std::to_string(ec.priority_level)
              << statusColor << std::setw(12) << ec.status << Color::RESET
              << std::setw(22) << ec.timestamp_logged;

    if (!ec.timestamp_processed.empty()) {
        std::cout << std::setw(22) << ec.timestamp_processed
                  << std::setw(10) << ec.ambulance_id;
    }

    std::cout << "\n";
}

void EmergencyManager::printAllCases() const {
    if (!head) {
        MessageHandler::info("No emergency cases available.");
        return;
    }

    std::cout << "\n--- All Emergency Cases ---\n";

    std::cout << std::left
              << std::setw(13)  << "Case_ID"
              << std::setw(13) << "Patient_ID"
              << std::setw(19) << "Patient_Name"
              << std::setw(29) << "Emergency_Type"
              << std::setw(10) << "Priority"
              << std::setw(12) << "Status"
              << std::setw(22) << "Logged"
              << std::setw(22) << "Processed"
              << std::setw(10) << "Ambulance"
              << "\n";
    std::cout << std::string(151, '-') << "\n";

    Node* current = head;
    while (current) {
        printCaseRow(current->data);
        current = current->next;
    }
}

// Print cases by status
void EmergencyManager::printCasesByStatus(const std::string& status) const {
    if (!head) {
        MessageHandler::info("No emergency cases available.");
        return;
    }

    std::cout << "\n--- " << status << " Cases ---\n";

    std::cout << std::left
              << std::setw(13)  << "Case_ID"
              << std::setw(13) << "Patient_ID"
              << std::setw(19) << "Patient_Name"
              << std::setw(29) << "Emergency_Type"
              << std::setw(10) << "Priority"
              << std::setw(12) << "Status"
              << std::setw(22) << "Logged"
              << std::setw(22) << "Processed"
              << std::setw(10) << "Ambulance"
              << "\n";
    std::cout << std::string(151, '-') << "\n"; 

    Node* current = head;
    bool found = false;
    while (current) {
        if (current->data.status == status) {
            printCaseRow(current->data);
            found = true;
        }
        current = current->next;
    }

    if (!found)
        std::cout << "(No " << status << " cases found)\n";
}

// Add case (insert by priority)
void EmergencyManager::addCase(const EmergencyCase& ec) {
    Node* newNode = new Node{ec, nullptr};

    // Empty list or higher priority than head
    if (!head || ec.priority_level < head->data.priority_level) {
        newNode->next = head;
        head = newNode;
        return;
    }

    Node* current = head;
    while (current->next && current->next->data.priority_level <= ec.priority_level) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}

// Pop the highest-priority case
EmergencyCase EmergencyManager::popHighestPriorityCase() {
    if (!head) {
        MessageHandler::error("No cases to pop!");
        return EmergencyCase{};
    }

    Node* tmp = head;
    EmergencyCase ec = head->data;
    head = head->next;
    delete tmp;

    return ec;
}

// Update case (used after processing)
void EmergencyManager::updateCase(const EmergencyCase& ec) {
    Node* current = head;
    while (current) {
        if (current->data.case_id == ec.case_id) {
            current->data = ec;
            return;
        }
        current = current->next;
    }
    addCase(ec);
}

// Generate next Case ID
std::string EmergencyManager::generateNextCaseID() {
    int maxID = 3100; // Start from a base, e.g., CASE-3101
    Node* current = head;

    while (current) {
        const std::string& id = current->data.case_id;
        // Assuming format is "CASE-XXXX"
        if (id.rfind("CASE-", 0) == 0) {
            try {
                int num = std::stoi(id.substr(5)); // Get number after "CASE-"
                if (num > maxID) maxID = num;
            } catch (...) {}
        }
    }

    std::ostringstream oss;
    oss << "CASE-" << (maxID + 1);
    return oss.str();
}