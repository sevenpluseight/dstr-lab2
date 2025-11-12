#ifndef EMERGENCY_MANAGER_HPP
#define EMERGENCY_MANAGER_HPP

#include <iostream>
#include <string>
#include "entities.hpp"
#include "message_handler.hpp"
#include "string_utils.hpp"
#include "path_utils.hpp"

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

    struct SupplyTypeNode {
        std::string typeName;
        SupplyTypeNode* next;
    };

    struct UniqueSupplyNode {
        std::string supplyName;
        std::string supplyType; // To link it to its type
        UniqueSupplyNode* next;
    };

    struct SupplyNode {
        std::string supplyID;   // Supply_Batch_ID
        std::string supplyName;
        std::string supplyType;
        SupplyNode* next;
    };

    struct AmbulanceNode {
        Ambulance data; // Use the struct from entities.hpp
        AmbulanceNode* next;
    };

    Node* head = nullptr; // Head of the emergency case list
    PatientNode* patientHead = nullptr; // Head of the patient data list
    TypeNode* typeHead = nullptr;
    SupplyTypeNode* supplyTypeHead = nullptr;
    UniqueSupplyNode* uniqueSupplyHead = nullptr;
    SupplyNode* supplyHead = nullptr;
    AmbulanceNode* ambHead = nullptr;

    // Private helper to add a patient to the list
    void addPatient(const std::string& id, const std::string& name);
    void addType(const std::string& type);

    void addSupplyType(const std::string& type);
    void addUniqueSupply(const std::string& name, const std::string& type);
    void addSupply(const std::string& id, const std::string& name, const std::string& type);

    bool isAmbulanceOnProcessingCase(const std::string& ambID) const;
    void clearAmbulanceList();

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
    
    int printSuppliesByType(const std::string& type) const;
    SupplyNode* getSupplyByTypeAndIndex(const std::string& type, int index) const;

    void addCase(const EmergencyCase& ec);

    EmergencyCase getHighestPriorityPendingCase() const;
    EmergencyCase popHighestPriorityPendingCase();

    void updateCase(const EmergencyCase& ec);
    std::string generateNextCaseID();
    bool isEmpty() const { return head == nullptr; }

    /**
     * @brief Loads ambulance data from the schedule CSV.
     * This clears any existing ambulance data in the manager.
     */
    void loadAmbulanceData(const std::string& filename);

    /**
     * @brief Finds the first ambulance that is "On Duty" or "Available".
     * @return The string ID of the ambulance, or "" if none are found.
     */
    std::string getFirstAvailableAmbulanceID() const;

    void loadSupplyData(const std::string& supplyDataFile);
    
    int printSupplyTypes() const;
    std::string getSupplyTypeByIndex(int index) const;

    int printUniqueSuppliesByType(const std::string& type) const;
    std::string getUniqueSupplyNameByTypeAndIndex(const std::string& type, int index) const;

    int printBatchesForSupply(const std::string& supplyName) const;
    SupplyNode* getBatchBySupplyNameAndIndex(const std::string& supplyName, int index) const;

    EmergencyCase* getCaseByID(const std::string& caseID);
    void logSupplyUsage(const EmergencyCase& ec, const std::string& supplyID, const std::string& supplyName, int quantity);
};

#endif // EMERGENCY_MANAGER_HPP