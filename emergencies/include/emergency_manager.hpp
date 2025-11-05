#ifndef EMERGENCY_MANAGER_HPP
#define EMERGENCY_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include "entities.hpp"
#include "message_handler.hpp"
#include "string_utils.hpp"
class EmergencyManager {
private:
    // Node structure for linked list
    struct Node {
        EmergencyCase data;
        Node* next;
    };

    Node* head = nullptr; 

public:
    EmergencyManager() = default;
    ~EmergencyManager();

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
