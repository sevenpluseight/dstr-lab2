// emergencies/include/emergency_manager.hpp
#ifndef EMERGENCY_MANAGER_HPP
#define EMERGENCY_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include "entities.hpp"
#include "message_handler.hpp"
#include "string_utils.hpp"

struct Node {
    EmergencyCase data;
    Node* next;
};

class EmergencyManager {
private:
    Node* head; // points to highest-priority case

public:
    EmergencyManager() : head(nullptr) {}
    ~EmergencyManager();

    void loadFromCSV(const std::string& filename);
    void saveToCSV(const std::string& filename);

    void printAllCases() const;
    void addCase(const EmergencyCase& ec);
    EmergencyCase popHighestPriorityCase();
    void updateCase(const EmergencyCase& ec);
    bool isEmpty() const { return head == nullptr; }
};

#endif
