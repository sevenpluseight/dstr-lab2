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
    // Node structure for emergency types
    struct TypeNode {
        std::string typeName;
        TypeNode* next;
    };

    Node* head = nullptr; // Head of the emergency case list
    PatientNode* patientHead = nullptr; // Head of the patient data list
    TypeNode* typeHead = nullptr;

    // Private helper to add a patient to the list
    void addPatient(const std::string& id, const std::string& name);
    void addType(const std::string& type);

public:
    EmergencyManager() = default;
    ~EmergencyManager(); // Updated to clear all lists

    // Managing patient data
    void loadPatientData(const std::string& patientDataFile);
    std::string getPatientName(const std::string& patientID) const;

    // Managing emergency types
    bool typeExists(const std::string& type) const;
    int printAllTypes() const;
    std::string getTypeByIndex(int index) const;

    // Managing emergency cases
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