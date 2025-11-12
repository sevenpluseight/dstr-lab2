#ifndef AMBULANCE_DISPATCHER_HPP
#define AMBULANCE_DISPATCHER_HPP
#define MAX_SHIFTS 1000
#include <iostream>
#include <string>
#include "entities.hpp"

class AmbulanceDispatcher {
private:
    static const int MAX_AMBULANCES = 20; // Adjust based on dataset size
    Ambulance ambulanceQueue[MAX_AMBULANCES]; // circular queue of active-duty ambulances
    int front; // front index of queue
    int rear;  // rear index of queue
    int count; // number of ambulances in queue

    ShiftRecord shiftRecords[MAX_AMBULANCES]; // store shift history if needed
    int shiftCount;

public:
    AmbulanceDispatcher();

    void run(); // main entry point called after login

private:
    void displayMenu();
    void registerAmbulance();
    void rotateShift();
    void displaySchedule();
    void updateShift();
    void viewInventory();
    int findAmbulanceIndex(const std::string& id); // helper for updating shifts
    void loadScheduleFromCSV(const std::string& filename);
    void loadShiftDatasetFromCSV(const std::string& filename);
    void saveScheduleToCSV(const std::string& filename);
    void saveShiftDatasetToCSV(const std::string& filename);
};

#endif
