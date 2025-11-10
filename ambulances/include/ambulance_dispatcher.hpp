#ifndef AMBULANCE_DISPATCHER_HPP
#define AMBULANCE_DISPATCHER_HPP

#include <iostream>
#include <string>
#include "entities.hpp"
#include "message_handler.hpp"
#include "path_utils.hpp"

// Node for queue
struct AmbulanceNode {
    Ambulance data;
    AmbulanceNode* next;
};

// Node for stack (shift history)
struct ShiftNode {
    std::string ambulance_id;
    std::string driver_name;
    std::string shift_start;
    std::string shift_end;
    ShiftNode* next;
};

class AmbulanceDispatcher {
private:
    std::string scheduleFile;
    std::string shiftFile;

    // Queue of available ambulances
    AmbulanceNode* front = nullptr;
    AmbulanceNode* rear = nullptr;

    // Stack for shift history
    ShiftNode* shiftTop = nullptr;

    // Private helpers
    void loadSchedule();
    void saveSchedule();
    void loadShiftHistory();
    void saveShiftHistory();

    void displayMenu();
    void viewSchedule();
    void assignAmbulance();
    void viewShiftHistory();

    void enqueueAmbulance(const Ambulance& amb);
    Ambulance dequeueAmbulance();
    bool isQueueEmpty() const { return front == nullptr; }

    void pushShift(const ShiftNode& shift);
public:
    AmbulanceDispatcher(const std::string& schedulePath, const std::string& shiftPath);
    ~AmbulanceDispatcher();

    void run(); // Entry point
};

#endif
