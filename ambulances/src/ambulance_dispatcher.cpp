#include "ambulance_dispatcher.hpp"
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <iostream>

// Constructor
AmbulanceDispatcher::AmbulanceDispatcher() {
    front = 0;
    rear = -1;
    count = 0;
    shiftCount = 0;
}

// Main loop
void AmbulanceDispatcher::run() {
    loadScheduleFromCSV("data/ambulance_schedule.csv");
    loadShiftDatasetFromCSV("data/shift_history.csv");

    int choice;
    do {
        displayMenu();
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore(); // consume newline
        switch(choice) {
            case 1: registerAmbulance(); saveScheduleToCSV("data/ambulance_schedule.csv"); break;
            case 2: rotateShift(); saveScheduleToCSV("data/ambulance_schedule.csv"); break;
            case 3: displaySchedule(); break;
            case 4: updateShift(); saveScheduleToCSV("data/ambulance_schedule.csv"); break;
            case 5: viewInventory(); break;
            case 6:
                saveScheduleToCSV("data/ambulance_schedule.csv");
                saveShiftDatasetToCSV("data/shift_history.csv");
                std::cout << "Exiting Ambulance Dispatcher...\n";
                break;
            default: std::cout << "Invalid choice. Try again.\n";
        }
    } while(choice != 6);
}

// Display menu
void AmbulanceDispatcher::displayMenu() {
    std::cout << "\n=== Ambulance Dispatcher Menu ===\n";
    std::cout << "1. Register Ambulance\n";
    std::cout << "2. Rotate Ambulance Shift\n";
    std::cout << "3. Display Ambulance Schedule\n";
    std::cout << "4. Update Ambulance Shift\n";
    std::cout << "5. View Ambulance Inventory\n";
    std::cout << "6. Exit\n";
}

// Register a new ambulance
void AmbulanceDispatcher::registerAmbulance() {
    if (count >= MAX_AMBULANCES) {
        std::cout << "Maximum ambulance limit reached.\n";
        return;
    }

    rear = (rear + 1) % MAX_AMBULANCES;
    Ambulance &amb = ambulanceQueue[rear];

    std::cout << "Enter Ambulance ID: ";
    std::getline(std::cin, amb.ambulance_id);
    std::cout << "Enter Driver Name: ";
    std::getline(std::cin, amb.driver_name);
    std::cout << "Enter Shift Start (YYYY-MM-DD HH:MM): ";
    std::getline(std::cin, amb.shift_start);
    std::cout << "Enter Shift End (YYYY-MM-DD HH:MM): ";
    std::getline(std::cin, amb.shift_end);
    std::cout << "Enter Shift Duration (hours): ";
    std::cin >> amb.shift_duration;
    std::cin.ignore();

    amb.driver_status = "On Duty";
    amb.ambulance_status = "On Duty";
    amb.oxygen_tank = 2;
    amb.first_aid_kit = 1;
    amb.medicine_units = 5;
    amb.last_service_date = "2025-09-01";
    amb.next_rotation_time = amb.shift_end;
    amb.location = "Unknown";

    count++;
    std::cout << "Ambulance registered successfully!\n";
}

// Rotate all ambulance shifts for 24-hour coverage
void AmbulanceDispatcher::rotateShift() {
    if (count <= 1) {
        std::cout << "Not enough ambulances to rotate.\n";
        return;
    }

    int currHour = 0, currMin = 0;
    for (int i = 0; i < count; i++) {
        int idx = (front + i) % MAX_AMBULANCES;
        Ambulance &amb = ambulanceQueue[idx];

        char buf[32];
        #ifdef _WIN32
            sprintf_s(buf, sizeof(buf), "2025-11-03 %02d:%02d", currHour, currMin);
        #else
            snprintf(buf, sizeof(buf), "2025-11-03 %02d:%02d", currHour, currMin);
        #endif
        amb.shift_start = buf;

        int endHour = currHour + amb.shift_duration;
        int endDay = 0;
        if (endHour >= 24) {
            endHour -= 24;
            endDay = 1;
        }

        #ifdef _WIN32
            sprintf_s(buf, sizeof(buf), "2025-11-%02d %02d:%02d", 3 + endDay, endHour, currMin);
        #else
            snprintf(buf, sizeof(buf), "2025-11-%02d %02d:%02d", 3 + endDay, endHour, currMin);
        #endif
        amb.shift_end = buf;
        amb.next_rotation_time = amb.shift_end;

        if (i == 0) {
            amb.driver_status = "On Duty";
            amb.ambulance_status = "On Duty";
        } else {
            amb.driver_status = "Available";
            amb.ambulance_status = "Available";
        }

        currHour = endHour;
    }

    std::cout << "All ambulance shifts rotated for 24-hour coverage successfully.\n";
}

// Display schedule
void AmbulanceDispatcher::displaySchedule() {
    if (count == 0) {
        std::cout << "No ambulances in schedule.\n";
        return;
    }

    std::cout << "\n=== Current Ambulance Schedule ===\n";
    for (int i = 0; i < count; i++) {
        int idx = (front + i) % MAX_AMBULANCES;
        Ambulance &amb = ambulanceQueue[idx];
        std::cout << amb.ambulance_id << " | " << amb.driver_name
                  << " | " << amb.shift_start << " - " << amb.shift_end
                  << " | Duration: " << amb.shift_duration
                  << " | Status: " << amb.ambulance_status << "\n";
    }
}

// Helper: add hours safely
std::string addHoursToDatetime(const std::string& datetime, int hours) {
    std::tm tm = {};
    std::istringstream ss(datetime);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M");
    if (ss.fail()) return datetime;

    std::time_t t = std::mktime(&tm);
    t += hours * 3600;

    std::tm newTm = {};
    #ifdef _WIN32
    localtime_s(&newTm, &t);
    #else
        localtime_r(&t, &newTm);
    #endif

    char buf[32];
    #ifdef _WIN32
        sprintf_s(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d",
              newTm.tm_year + 1900, newTm.tm_mon + 1, newTm.tm_mday,
              newTm.tm_hour, newTm.tm_min);
    #else
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d",
              newTm.tm_year + 1900, newTm.tm_mon + 1, newTm.tm_mday,
              newTm.tm_hour, newTm.tm_min);
    #endif

    return std::string(buf);
}

// Update all ambulance shifts sequentially and refresh shiftRecords
void AmbulanceDispatcher::updateShift() {
    std::string startDatetime;
    int newDuration;

    std::cout << "Enter new Shift Start (YYYY-MM-DD HH:MM) for the first ambulance: ";
    std::getline(std::cin, startDatetime);
    std::cout << "Enter new Shift Duration (hours): ";
    std::cin >> newDuration;
    std::cin.ignore();

    std::string currentStart = startDatetime;

    // Clear previous shift records for this update
    shiftCount = 0;

    for (int i = 0; i < count; i++) {
        int idx = (front + i) % MAX_AMBULANCES;
        Ambulance &amb = ambulanceQueue[idx];

        // Save old shift to history
        amb.shift_history.appendArray(amb.shift_start + " - " + amb.shift_end);

        // Assign new shift
        amb.shift_start = currentStart;
        amb.shift_end = addHoursToDatetime(currentStart, newDuration);
        amb.shift_duration = newDuration;
        amb.next_rotation_time = amb.shift_end;

        if (i == 0) {
            amb.driver_status = "On Duty";
            amb.ambulance_status = "On Duty";
        } else {
            amb.driver_status = "Available";
            amb.ambulance_status = "Available";
        }

        // Add new shift record
        ShiftRecord &sr = shiftRecords[shiftCount++];
        sr.shift_id = "SHIFT-" + std::to_string(2000 + shiftCount); // unique ID
        sr.ambulance_id = amb.ambulance_id;
        sr.driver_name = amb.driver_name;
        sr.shift_start = amb.shift_start;
        sr.shift_end = amb.shift_end;
        sr.shift_duration_hours = amb.shift_duration;
        sr.assigned_case_id = amb.assigned_case_id.empty() ? "-" : amb.assigned_case_id;
        sr.cases_handled = 0;
        sr.total_distance_km = 0.0;
        sr.oxygen_used = 0;
        sr.medicine_used = 0;
        sr.status_at_end = "Scheduled";

        // Prepare start time for next ambulance
        currentStart = amb.shift_end;
    }

    std::cout << "All ambulance shifts updated successfully (schedule + shift dataset refreshed).\n";
}


// View inventory
void AmbulanceDispatcher::viewInventory() {
    if (count == 0) {
        std::cout << "No ambulances available.\n";
        return;
    }

    std::cout << "\n=== Ambulance Inventory ===\n";
    for (int i = 0; i < count; i++) {
        int idx = (front + i) % MAX_AMBULANCES;
        Ambulance &amb = ambulanceQueue[idx];
        std::cout << amb.ambulance_id << " | Oxygen: " << amb.oxygen_tank
                  << " | First Aid Kits: " << amb.first_aid_kit
                  << " | Medicine Units: " << amb.medicine_units << "\n";
    }
}

// Find ambulance
int AmbulanceDispatcher::findAmbulanceIndex(const std::string& id) {
    for (int i = 0; i < count; i++) {
        int idx = (front + i) % MAX_AMBULANCES;
        if (ambulanceQueue[idx].ambulance_id == id) return idx;
    }
    return -1;
}

// Load schedule from CSV
void AmbulanceDispatcher::loadScheduleFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Warning: Unable to open " << filename << ". Starting empty.\n";
        return;
    }

    std::string line;
    std::getline(file, line); // skip header
    count = 0; front = 0; rear = -1;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        Ambulance amb;

        std::getline(ss, amb.ambulance_id, ',');
        std::getline(ss, amb.driver_name, ',');
        std::getline(ss, amb.driver_status, ',');
        std::getline(ss, amb.shift_start, ',');
        std::getline(ss, amb.shift_end, ',');
        std::getline(ss, token, ','); amb.shift_duration = std::stoi(token);
        std::getline(ss, amb.next_rotation_time, ',');
        std::getline(ss, amb.assigned_case_id, ',');
        std::getline(ss, amb.ambulance_status, ',');
        std::getline(ss, token, ','); amb.oxygen_tank = std::stoi(token);
        std::getline(ss, token, ','); amb.first_aid_kit = std::stoi(token);
        std::getline(ss, token, ','); amb.medicine_units = std::stoi(token);
        std::getline(ss, amb.last_service_date, ',');
        std::getline(ss, amb.location, ',');

        rear = (rear + 1) % MAX_AMBULANCES;
        ambulanceQueue[rear] = amb;
        count++;
    }

    file.close();
    std::cout << "Loaded " << count << " ambulances from " << filename << ".\n";
}

// Load shift records
void AmbulanceDispatcher::loadShiftDatasetFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Warning: Unable to open " << filename << ". Starting empty.\n";
        return;
    }

    std::string line;
    std::getline(file, line); // skip header
    shiftCount = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        ShiftRecord sr;

        std::getline(ss, sr.shift_id, ',');
        std::getline(ss, sr.ambulance_id, ',');
        std::getline(ss, sr.driver_name, ',');
        std::getline(ss, sr.shift_start, ',');
        std::getline(ss, sr.shift_end, ',');
        std::getline(ss, token, ','); sr.shift_duration_hours = std::stoi(token);
        std::getline(ss, sr.assigned_case_id, ',');
        std::getline(ss, token, ','); sr.cases_handled = std::stoi(token);
        std::getline(ss, token, ','); sr.total_distance_km = std::stod(token);
        std::getline(ss, token, ','); sr.oxygen_used = std::stoi(token);
        std::getline(ss, token, ','); sr.medicine_used = std::stoi(token);
        std::getline(ss, sr.status_at_end, ',');

        shiftRecords[shiftCount++] = sr;
    }

    file.close();
    std::cout << "Loaded " << shiftCount << " shift records from " << filename << ".\n";
}

// Save schedule to CSV
void AmbulanceDispatcher::saveScheduleToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Unable to open " << filename << " for saving.\n";
        return;
    }

    file << "Ambulance_ID,Driver_Name,Driver_Status,Shift_Start,Shift_End,Shift_Duration_Hours,"
            "Next_Rotation_Time,Assigned_Case_ID,Ambulance_Status,OxygenTank,FirstAidKit,"
            "MedicineUnits,Last_Service_Date,Location\n";

    for (int i = 0; i < count; i++) {
        int idx = (front + i) % MAX_AMBULANCES;
        Ambulance &amb = ambulanceQueue[idx];

        file << amb.ambulance_id << ","
             << amb.driver_name << ","
             << amb.driver_status << ","
             << amb.shift_start << ","
             << amb.shift_end << ","
             << amb.shift_duration << ","
             << amb.next_rotation_time << ","
             << amb.assigned_case_id << ","
             << amb.ambulance_status << ","
             << amb.oxygen_tank << ","
             << amb.first_aid_kit << ","
             << amb.medicine_units << ","
             << amb.last_service_date << ","
             << amb.location << "\n";
    }

    file.close();
    std::cout << "Schedule saved to " << filename << " successfully.\n";
}

// Save shift dataset
void AmbulanceDispatcher::saveShiftDatasetToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Unable to open " << filename << " for saving.\n";
        return;
    }

    file << "Shift_ID,Ambulance_ID,Driver_Name,Shift_Start,Shift_End,Shift_Duration_Hours,"
            "Assigned_Case_ID,Cases_Handled,Total_Distance_km,OxygenUsed,MedicineUsed,Status_At_End\n";

    for (int i = 0; i < shiftCount; i++) {
        const ShiftRecord &s = shiftRecords[i];
        file << s.shift_id << "," << s.ambulance_id << "," << s.driver_name << ","
             << s.shift_start << "," << s.shift_end << "," << s.shift_duration_hours << ","
             << s.assigned_case_id << "," << s.cases_handled << "," << s.total_distance_km << ","
             << s.oxygen_used << "," << s.medicine_used << "," << s.status_at_end << "\n";
    }

    file.close();
    std::cout << "Shift dataset saved to " << filename << " successfully.\n";
}
