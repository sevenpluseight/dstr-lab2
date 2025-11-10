#include <iostream>
#include <iomanip>
#include <algorithm>
#include "../../ambulances/include/ambulance_dispatcher.hpp"

using namespace std;

class MessageHandler {
public:
    static void info(const string& msg) { cout << msg << endl; }
    static void error(const string& msg) { cout << "Error: " << msg << endl; }
};

void AmbulanceDispatcher::displayMenu() {
    int choice = 0;
    do {
        cout << "\n=== Ambulance Dispatcher Menu ===\n";
        cout << "1. View Ambulances\n";
        cout << "2. Add Ambulance\n";
        cout << "3. Remove Ambulance\n";
        cout << "4. Dispatch Ambulance\n";
        cout << "5. Rotate Ambulance Shift\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice) {
            case 1: displaySchedule(); break;
            case 2: registerAmbulance(); break;
            case 3: removeAmbulance(); break;
            case 4: dispatchAmbulance(); break;
            case 5: rotateAmbulanceShift(); break;
            case 6: MessageHandler::info("Returning to login...\n"); break;
            default: MessageHandler::error("Invalid choice!"); break;
        }
    } while (choice != 6);
}

// ---------------- View Schedule ----------------
void AmbulanceDispatcher::displaySchedule() const {
    if (ambulances.empty()) {
        cout << "\n⚠️ No ambulance records found.\n";
        return;
    }

    cout << "\n===== 🚑 Current Ambulance Schedule =====\n";
    cout << left << setw(10) << "ID"
         << setw(15) << "Driver"
         << setw(12) << "DrvStat"
         << setw(20) << "Shift Start"
         << setw(20) << "Shift End"
         << setw(10) << "Hours"
         << setw(15) << "Case ID"
         << setw(15) << "AmbStat"
         << setw(15) << "Location" << endl;
    cout << string(140, '-') << endl;

    for (const auto& amb : ambulances) {
        cout << left << setw(10) << amb.ambulance_id
             << setw(15) << amb.driver_name
             << setw(12) << amb.driver_status
             << setw(20) << amb.shift_start
             << setw(20) << amb.shift_end
             << setw(10) << amb.shift_duration
             << setw(15) << amb.assigned_case_id
             << setw(15) << amb.ambulance_status
             << setw(15) << amb.location << endl;
    }
    cout << endl;
}

// ---------------- Add Ambulance ----------------
void AmbulanceDispatcher::registerAmbulance() {
    Ambulance amb;
    cout << "\nEnter new Ambulance details:\n";
    cout << "Ambulance ID: "; cin >> amb.ambulance_id;
    cout << "Driver Name: "; cin.ignore(); getline(cin, amb.driver_name);
    cout << "Driver Status (Available/OnDuty/Unavailable): "; getline(cin, amb.driver_status);
    cout << "Shift Start (YYYY-MM-DD HH:MM): "; getline(cin, amb.shift_start);
    cout << "Shift End (YYYY-MM-DD HH:MM): "; getline(cin, amb.shift_end);
    cout << "Shift Duration (hours): "; cin >> amb.shift_duration;
    cout << "Assigned Case ID (if any, '-' if none): "; cin >> amb.assigned_case_id;
    cout << "Ambulance Status (Available/On Duty/Under Maintenance): "; cin >> amb.ambulance_status;
    cout << "Location: "; cin.ignore(); getline(cin, amb.location);

    ambulances.push_back(amb);
    cout << "\n✅ Ambulance " << amb.ambulance_id << " successfully registered!\n";
}

// ---------------- Remove Ambulance ----------------
void AmbulanceDispatcher::removeAmbulance() {
    if (ambulances.empty()) {
        MessageHandler::info("No ambulances to remove.\n");
        return;
    }

    string id;
    cout << "Enter Ambulance ID to remove: ";
    cin >> id;

    auto it = find_if(ambulances.begin(), ambulances.end(),
                      [&id](const Ambulance& amb){ return amb.ambulance_id == id; });

    if (it != ambulances.end()) {
        ambulances.erase(it);
        MessageHandler::info("Ambulance removed successfully.\n");
    } else {
        MessageHandler::error("Ambulance ID not found!");
    }
}

// ---------------- Dispatch Ambulance (stub) ----------------
void AmbulanceDispatcher::dispatchAmbulance() {
    MessageHandler::info("Dispatching an ambulance (functionality not implemented yet).");
}

// ---------------- Rotate Shift ----------------
void AmbulanceDispatcher::rotateAmbulanceShift() {
    if (ambulances.empty()) {
        MessageHandler::info("No ambulances to rotate.\n");
        return;
    }

    rotate(ambulances.begin(), ambulances.begin() + 1, ambulances.end());
    MessageHandler::info("Ambulance shift rotation completed.\n");
}

// ---------------- Sample Data ----------------
void AmbulanceDispatcher::loadSampleData() {
    ambulances.push_back({"AMB001", "John Lim", "OnDuty", "2025-11-03 08:00", "2025-11-03 16:00", 8, "CASE-3101", "On Duty", "City Center"});
    ambulances.push_back({"AMB002", "Amira Tan", "Available", "2025-11-03 16:00", "2025-11-03 21:00", 5, "-", "Available", "Hospital HQ"});
    ambulances.push_back({"AMB003", "Raj Kumar", "Unavailable", "2025-11-03 09:00", "2025-11-03 17:00", 8, "-", "Unavailable", "Maintenance Bay"});
}
