#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "emergency_manager.hpp"

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
        std::getline(ss, ec.patient_name, ',');
        std::getline(ss, ec.emergency_type, ',');
        std::string prio;
        std::getline(ss, prio, ',');
        ec.priority_level = std::stoi(prio);
        std::getline(ss, ec.status, ',');
        std::getline(ss, ec.timestamp_logged, ',');
        std::getline(ss, ec.timestamp_processed, ',');
        std::getline(ss, ec.ambulance_id, ',');

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

// Print all cases
void EmergencyManager::printAllCases() const {
    if (!head) {
        MessageHandler::info("No emergency cases available.");
        return;
    }

    std::cout << "\n--- Emergency Cases ---\n";
    Node* current = head;
    while (current) {
        const EmergencyCase& ec = current->data;
        std::cout << ec.case_id << " | " << ec.patient_name
                  << " | " << ec.emergency_type
                  << " | Priority: " << ec.priority_level
                  << " | Status: " << ec.status << "\n";
        current = current->next;
    }
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
    // If not found, add it (e.g., reinsert after processing if needed)
    addCase(ec);
}
