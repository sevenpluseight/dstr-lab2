#ifndef AMBULANCE_DISPATCHER_HPP
#define AMBULANCE_DISPATCHER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include "../../utils/entities.hpp"
#include "../../utils/dynamic_array.hpp"

#define MAX_AMBULANCES 50
#define MAX_SHIFTS 500

/**
 * @brief Handles ambulance operations:
 * - View queue
 * - Register/update drivers
 * - Edit shift hours
 * - Manage availability
 * - View supplies (read-only)
 * - View emergency stats (day/week/year)
 * - Auto-save data on exit
 */
class AmbulanceDispatcher {
private:
    DynamicArray ambulanceData;
    DynamicArray shiftHistory;

    Ambulance ambulances[MAX_AMBULANCES];
    int ambulanceCount;

    // Utility
    void loadAmbulanceData(const std::string &path);
    void saveAmbulanceData(const std::string &path);
    void loadShiftHistory(const std::string &path);

    // Functionalities
    void viewAmbulanceQueue();
    void registerDriver();
    void editShiftHours();
    void updateDriverAvailability();
    void viewSuppliesReadOnly();
    void viewEmergencyStats();

    // Helpers
    void recalcShiftRotation(int hours);
    int getCasesHandled(const std::string &ambID, const std::string &periodType);

public:
    AmbulanceDispatcher();
    void run();
};

#endif
