#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "emergency_manager.hpp"
#include "path_utils.hpp"

#include "../utils/path_utils.hpp" // for getDataFilePath if needed

std::string getPatientNameByID(const std::string& patientID) {
    std::string patientFile = getDataFilePath("patient_data.csv");
    std::ifstream file(patientFile);
    if (!file.is_open()) {
        MessageHandler::warning("patient_data.csv not found.");
        return "Unknown";
    }

    std::string line;
    std::getline(file, line); // skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, name;
        std::getline(ss, id, ',');
        std::getline(ss, name, ','); // 2nd column is Name
        if (id == patientID) {
            file.close();
            return name;
        }
    }

    file.close();
    return "Unknown";
}

namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[31m"; // Pending
    const std::string YELLOW  = "\033[33m"; // Processing
    const std::string GREEN   = "\033[32m"; // Completed
}

EmergencyManager::~EmergencyManager() {
    Node* current = head;
    while (current) {
        Node* tmp = current;
        current = current->next;
        delete tmp;
    }
}

// Load from CSV
void EmergencyManager::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        MessageHandler::warning("Emergency CSV not found, starting empty.");
        return;
    }

    std::string line;
    std::getline(file, line); // skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        EmergencyCase ec;
        std::getline(ss, ec.case_id, ',');
        std::getline(ss, ec.patient_id, ',');
        std::getline(ss, ec.emergency_type, ',');
        std::string prio;
        std::getline(ss, prio, ',');
        ec.priority_level = std::stoi(prio);
        std::getline(ss, ec.status, ',');
        std::getline(ss, ec.timestamp_logged, ',');
        std::getline(ss, ec.timestamp_processed, ',');
        std::getline(ss, ec.ambulance_id, ',');

        ec.patient_name = getPatientNameByID(ec.patient_id);

        addCase(ec); // insert in priority order
    }
    file.close();
}

// Save to CSV
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

// Helper function to print a single case row
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
              << std::setw(12)  << ec.case_id
              << std::setw(12) << ec.patient_id
              << std::setw(18) << ec.patient_name
              << std::setw(28) << ec.emergency_type
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
              << std::setw(12)  << "CaseID"
              << std::setw(12) << "PatientID"
              << std::setw(18) << "PatientName"
              << std::setw(28) << "EmergencyType"
              << std::setw(10) << "Priority"
              << std::setw(12) << "Status"
              << std::setw(22) << "Logged"
              << std::setw(22) << "Processed"
              << std::setw(10) << "Ambulance"
              << "\n";
    std::cout << std::string(143, '-') << "\n";

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
              << std::setw(12)  << "CaseID"
              << std::setw(12) << "PatientID"
              << std::setw(18) << "PatientName"
              << std::setw(28) << "EmergencyType"
              << std::setw(10) << "Priority"
              << std::setw(12) << "Status"
              << std::setw(22) << "Logged"
              << std::setw(22) << "Processed"
              << std::setw(10) << "Ambulance"
              << "\n";
    std::cout << std::string(143, '-') << "\n"; 

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

std::string EmergencyManager::generateNextCaseID() {
    int maxID = 0;
    Node* current = head;

    while (current) {
        const std::string& id = current->data.case_id;
        if (id.rfind("C", 0) == 0) {
            try {
                int num = std::stoi(id.substr(1));
                if (num > maxID) maxID = num;
            } catch (...) {}
        }
        current = current->next;
    }

    std::ostringstream oss;
    oss << "C" << std::setw(4) << std::setfill('0') << (maxID + 1);
    return oss.str();
}
