#include "ambulance_dispatcher.hpp"
#include "time_utils.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib> // rand()

// Constructor
AmbulanceDispatcher::AmbulanceDispatcher(const std::string& schedulePath,
                                         const std::string& shiftPath)
    : scheduleFile(schedulePath), shiftFile(shiftPath)
{
    loadSchedule();
    loadShiftHistory();
}

// Destructor
AmbulanceDispatcher::~AmbulanceDispatcher() {
    // Clear queue
    while (!isQueueEmpty()) {
        dequeueAmbulance();
    }

    // Clear stack
    while (shiftTop) {
        ShiftNode* tmp = shiftTop;
        shiftTop = shiftTop->next;
        delete tmp;
    }
}

// Queue operations
void AmbulanceDispatcher::enqueueAmbulance(const Ambulance& amb) {
    AmbulanceNode* newNode = new AmbulanceNode{amb, nullptr};
    if (!rear) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

Ambulance AmbulanceDispatcher::dequeueAmbulance() {
    if (isQueueEmpty()) {
        return Ambulance{};
    }
    AmbulanceNode* tmp = front;
    Ambulance amb = tmp->data;
    front = front->next;
    if (!front) rear = nullptr;
    delete tmp;
    return amb;
}

// Stack operations
void AmbulanceDispatcher::pushShift(const ShiftNode& shift) {
    ShiftNode* newNode = new ShiftNode(shift);
    newNode->next = shiftTop;
    shiftTop = newNode;
}

// Load ambulance schedule from CSV
void AmbulanceDispatcher::loadSchedule() {
    std::ifstream file(scheduleFile);
    if (!file.is_open()) {
        MessageHandler::warning("Schedule CSV not found, starting empty.");
        return;
    }

    std::string line;
    std::getline(file, line); // skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Ambulance amb;
        std::string duration;

        std::getline(ss, amb.ambulance_id, ',');
        std::getline(ss, amb.driver_name, ',');
        std::getline(ss, amb.driver_status, ',');
        std::getline(ss, amb.shift_start, ',');
        std::getline(ss, amb.shift_end, ',');
        std::getline(ss, duration, ',');
        amb.shift_duration = duration.empty() ? 0 : std::stoi(duration);
        std::getline(ss, amb.assigned_case_id, ',');
        std::getline(ss, amb.ambulance_status, ',');
        std::getline(ss, amb.location, ',');

        // Only enqueue available ambulances
        if (amb.ambulance_status == "Available" && amb.driver_status == "Available") {
            enqueueAmbulance(amb);
        }
    }

    file.close();
}

// Save ambulance schedule
void AmbulanceDispatcher::saveSchedule() {
    std::ofstream file(scheduleFile);
    file << "ambulance_id,driver_name,driver_status,shift_start,shift_end,shift_duration,assigned_case_id,ambulance_status,location\n";
    AmbulanceNode* current = front;
    while (current) {
        const Ambulance& amb = current->data;
        file << amb.ambulance_id << "," << amb.driver_name << "," << amb.driver_status << ","
             << amb.shift_start << "," << amb.shift_end << "," << amb.shift_duration << ","
             << amb.assigned_case_id << "," << amb.ambulance_status << "," << amb.location << "\n";
        current = current->next;
    }
    file.close();
}

// Load shift history
void AmbulanceDispatcher::loadShiftHistory() {
    std::ifstream file(shiftFile);
    if (!file.is_open()) return;

    std::string line;
    std::getline(file, line); // skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        ShiftNode shift;
        std::getline(ss, shift.ambulance_id, ',');
        std::getline(ss, shift.driver_name, ',');
        std::getline(ss, shift.shift_start, ',');
        std::getline(ss, shift.shift_end, ',');
        shift.next = nullptr;
        pushShift(shift);
    }
    file.close();
}

// Save shift history
void AmbulanceDispatcher::saveShiftHistory() {
    std::ofstream file(shiftFile);
    file << "ambulance_id,driver_name,shift_start,shift_end\n";
    ShiftNode* current = shiftTop;
    while (current) {
        file << current->ambulance_id << "," << current->driver_name << ","
             << current->shift_start << "," << current->shift_end << "\n";
        current = current->next;
    }
}

// Dispatcher Menu
void AmbulanceDispatcher::displayMenu() {
    while (true) {
        std::cout << "\n--- Ambulance Dispatcher Menu ---\n";
        std::cout << "1. View available ambulances\n";
        std::cout << "2. Assign ambulance\n";
        std::cout << "3. View shift history\n";
        std::cout << "4. Exit\n";
        std::cout << "Select an option: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") viewSchedule();
        else if (choice == "2") assignAmbulance();
        else if (choice == "3") viewShiftHistory();
        else if (choice == "4") break;
        else MessageHandler::warning("Invalid option, please try again.");
    }
}

// View schedule
void AmbulanceDispatcher::viewSchedule() {
    if (isQueueEmpty()) {
        MessageHandler::info("No available ambulances.");
        return;
    }
    std::cout << "\n--- Available Ambulances ---\n";
    AmbulanceNode* current = front;
    while (current) {
        const Ambulance& amb = current->data;
        std::cout << amb.ambulance_id << " | Driver: " << amb.driver_name
                  << " | Status: " << amb.driver_status
                  << " | Location: " << amb.location << "\n";
        current = current->next;
    }
}

// Assign ambulance
void AmbulanceDispatcher::assignAmbulance() {
    if (isQueueEmpty()) {
        MessageHandler::warning("No available ambulances to assign.");
        return;
    }

    Ambulance amb = dequeueAmbulance();
    amb.assigned_case_id = "CASE-" + std::to_string(rand() % 1000 + 1);
    amb.ambulance_status = "On Duty";
    amb.driver_status = "OnDuty";

    MessageHandler::info("Ambulance " + amb.ambulance_id + " assigned to case " + amb.assigned_case_id);

    // Record in shift history
    ShiftNode shift;
    shift.ambulance_id = amb.ambulance_id;
    shift.driver_name = amb.driver_name;
    shift.shift_start = getCurrentTimestamp();
    shift.shift_end = ""; // shift ongoing
    shift.next = nullptr;
    pushShift(shift);

    saveShiftHistory();
    saveSchedule();
}

// View shift history
void AmbulanceDispatcher::viewShiftHistory() {
    if (!shiftTop) {
        MessageHandler::info("No shift history available.");
        return;
    }

    std::cout << "\n--- Shift History (most recent first) ---\n";
    ShiftNode* current = shiftTop;
    while (current) {
        std::cout << current->ambulance_id << " | Driver: " << current->driver_name
                  << " | Start: " << current->shift_start
                  << " | End: " << (current->shift_end.empty() ? "Ongoing" : current->shift_end)
                  << "\n";
        current = current->next;
    }
}

// Run entry
void AmbulanceDispatcher::run() {
    displayMenu();
}
