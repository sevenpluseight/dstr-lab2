#ifndef EMERGENCY_MANAGER_HPP
#define EMERGENCY_MANAGER_HPP

#include <iostream>
#include <string>
#include "entities.hpp"
#include "message_handler.hpp"
#include "string_utils.hpp"

class EmergencyManager {
private:
    // Node structure for emergency cases
    struct Node {
        EmergencyCase data;
        Node* next;
    };

    // Node structure for patient data
    struct PatientNode {
        std::string patientID;
        std::string patientName;
        PatientNode* next;
    };

    Node* head = nullptr; // Head of the emergency case list
    PatientNode* patientHead = nullptr; // Head of the patient data list

    // Private helper to add a patient to the list
    void addPatient(const std::string& id, const std::string& name);

public:
    EmergencyManager() = default;
    ~EmergencyManager(); // Updated to clear both lists

    // --- New functions for managing patient data ---
    void loadPatientData(const std::string& patientDataFile);
    std::string getPatientName(const std::string& patientID) const;
    // --- End new functions ---

    void loadFromCSV(const std::string& filename);
    void saveToCSV(const std::string& filename);
    void printAllCases() const;
    void printCasesByStatus(const std::string& status) const;
    void addCase(const EmergencyCase& ec);
    EmergencyCase popHighestPriorityCase();
    void updateCase(const EmergencyCase& ec);

    std::string generateNextCaseID();

    bool isEmpty() const { return head == nullptr; }
};

#endif // EMERGENCY_MANAGER_HPP