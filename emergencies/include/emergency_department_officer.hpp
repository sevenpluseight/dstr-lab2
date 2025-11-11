#ifndef EMERGENCY_DEPARTMENT_OFFICER_HPP
#define EMERGENCY_DEPARTMENT_OFFICER_HPP

#include <iostream>
#include <string>
#include "emergency_manager.hpp"
#include "path_utils.hpp" 

class EmergencyDepartmentOfficer {
private:
    EmergencyManager manager;
    std::string dataFile;
    std::string patientDataFile; 

    void displayMenu();
    void viewCases();
    void addCase();
    void processHighestPriorityCase();

    void completeProcessingCase();

public:
    EmergencyDepartmentOfficer();
    void run();  // Entry point after login
};

#endif