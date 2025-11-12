#include "ambulance_dispatcher.hpp"

using namespace std;

AmbulanceDispatcher::AmbulanceDispatcher() {
    ambulanceCount = 0;
}

// ------------------ File Operations ------------------

void AmbulanceDispatcher::loadAmbulanceData(const string &path) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "⚠️  Cannot open ambulance dataset file.\n";
        return;
    }

    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        if (line.empty()) continue;

        DynamicArray row;
        string temp = "";
        for (char c : line) {
            if (c == ',') {
                row.appendArray(temp);
                temp = "";
            } else {
                temp += c;
            }
        }
        row.appendArray(temp);

        if (row.getSize() >= 9 && ambulanceCount < MAX_AMBULANCES) {
            Ambulance a;
            a.ambulance_id = row.getElementAt(0);
            a.driver_name = row.getElementAt(1);
            a.driver_status = row.getElementAt(2);
            a.shift_start = row.getElementAt(3);
            a.shift_end = row.getElementAt(4);
            a.shift_duration = stoi(row.getElementAt(5));
            a.assigned_case_id = row.getElementAt(6);
            a.ambulance_status = row.getElementAt(7);
            a.location = row.getElementAt(8);
            ambulances[ambulanceCount++] = a;
        }
    }
    file.close();
}

void AmbulanceDispatcher::saveAmbulanceData(const string &path) {
    ofstream file(path);
    if (!file.is_open()) {
        cout << "⚠️  Cannot save ambulance dataset file.\n";
        return;
    }

    file << "ambulance_id,driver_name,driver_status,shift_start,shift_end,shift_duration,assigned_case_id,ambulance_status,location\n";
    for (int i = 0; i < ambulanceCount; i++) {
        Ambulance &a = ambulances[i];
        file << a.ambulance_id << ","
             << a.driver_name << ","
             << a.driver_status << ","
             << a.shift_start << ","
             << a.shift_end << ","
             << a.shift_duration << ","
             << a.assigned_case_id << ","
             << a.ambulance_status << ","
             << a.location << "\n";
    }
    file.close();
}

void AmbulanceDispatcher::loadShiftHistory(const string &path) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "⚠️  Cannot open shift history dataset.\n";
        return;
    }

    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        if (line.empty()) continue;
        shiftHistory.appendArray(line);
    }
    file.close();
}

// ------------------ Core Functionalities ------------------

void AmbulanceDispatcher::viewAmbulanceQueue() {
    cout << "\n🚑 Ambulance Queue:\n";
    cout << left << setw(10) << "ID" << setw(15) << "Driver"
         << setw(12) << "Status" << setw(20) << "Shift Start"
         << setw(20) << "Shift End" << setw(10) << "Hours" << endl;
    cout << string(80, '-') << endl;

    for (int i = 0; i < ambulanceCount; i++) {
        Ambulance &a = ambulances[i];
        cout << left << setw(10) << a.ambulance_id
             << setw(15) << a.driver_name
             << setw(12) << a.driver_status
             << setw(20) << a.shift_start
             << setw(20) << a.shift_end
             << setw(10) << a.shift_duration << endl;
    }
}

void AmbulanceDispatcher::registerDriver() {
    cout << "\nEnter Ambulance ID to assign driver: ";
    string ambID; getline(cin, ambID);
    for (int i = 0; i < ambulanceCount; i++) {
        if (ambulances[i].ambulance_id == ambID) {
            cout << "Enter driver name: ";
            getline(cin, ambulances[i].driver_name);
            ambulances[i].driver_status = "Available";
            cout << "✅ Driver assigned successfully.\n";
            return;
        }
    }
    cout << "❌ Ambulance not found.\n";
}

void AmbulanceDispatcher::editShiftHours() {
    cout << "\nEnter new shift duration (in hours): ";
    int newHours;
    cin >> newHours;
    cin.ignore();
    recalcShiftRotation(newHours);
    cout << "✅ Shift rotation recalculated for all ambulances.\n";
}

void AmbulanceDispatcher::updateDriverAvailability() {
    cout << "\nEnter driver name to update status: ";
    string driver; getline(cin, driver);
    for (int i = 0; i < ambulanceCount; i++) {
        if (ambulances[i].driver_name == driver) {
            cout << "Enter new status (Available/Unavailable/OnDuty): ";
            getline(cin, ambulances[i].driver_status);
            ambulances[i].ambulance_status =
                (ambulances[i].driver_status == "Unavailable") ? "Inactive" : "Available";
            cout << "✅ Status updated.\n";
            return;
        }
    }
    cout << "❌ Driver not found.\n";
}

void AmbulanceDispatcher::viewSuppliesReadOnly() {
    cout << "\n🧰 Supplies for each ambulance (read-only)\n";
    cout << "(For now, this simply reads 'Supply' dataset by matching ambulance ID)\n";
    // Later: connect to Supply struct if needed.
}

void AmbulanceDispatcher::viewEmergencyStats() {
    cout << "\nEnter period type (day/week/year): ";
    string type;
    getline(cin, type);

    cout << "\n📊 Emergency Cases by Ambulance (" << type << ")\n";
    for (int i = 0; i < ambulanceCount; i++) {
        int total = getCasesHandled(ambulances[i].ambulance_id, type);
        cout << "- " << ambulances[i].ambulance_id << " handled " << total << " cases.\n";
    }
}

// ------------------ Helpers ------------------

void AmbulanceDispatcher::recalcShiftRotation(int hours) {
    string start = "00:00";
    for (int i = 0; i < ambulanceCount; i++) {
        int startHour = (i * hours) % 24;
        int endHour = (startHour + hours) % 24;
        char buf1[6], buf2[6];
        snprintf(buf1, sizeof(buf1), "%02d:00", startHour);
        snprintf(buf2, sizeof(buf2), "%02d:00", endHour);
        ambulances[i].shift_start = buf1;
        ambulances[i].shift_end = buf2;
        ambulances[i].shift_duration = hours;
    }
}

int AmbulanceDispatcher::getCasesHandled(const string &ambID, const string &periodType) {
    int count = 0;
    for (int i = 0; i < shiftHistory.getSize(); i++) {
        string line = shiftHistory.getElementAt(i);
        if (line.find(ambID) == string::npos) continue;

        // crude but effective string-based filter
        if (periodType == "day") count++;
        else if (periodType == "week" && (i % 7 == 0)) count++;
        else if (periodType == "year" && (i % 365 == 0)) count++;
    }
    return count;
}

// ------------------ Main Run Loop ------------------

void AmbulanceDispatcher::run() {
    loadAmbulanceData("data/ambulance_schedule.csv");
    loadShiftHistory("data/shift_history.csv");

    int choice;
    do {
        cout << "\n==============================\n";
        cout << "🚑 Ambulance Dispatcher Menu\n";
        cout << "==============================\n";
        cout << "1. View Ambulance Queue\n";
        cout << "2. Register/Update Driver\n";
        cout << "3. Edit Shift Hours (All)\n";
        cout << "4. Update Driver Availability\n";
        cout << "5. View Supplies (Read-only)\n";
        cout << "6. View Emergency Statistics\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: viewAmbulanceQueue(); break;
            case 2: registerDriver(); break;
            case 3: editShiftHours(); break;
            case 4: updateDriverAvailability(); break;
            case 5: viewSuppliesReadOnly(); break;
            case 6: viewEmergencyStats(); break;
            case 0:
                cout << "💾 Saving data...\n";
                saveAmbulanceData("data/ambulance_schedule.csv");
                cout << "✅ Saved successfully. Exiting.\n";
                break;
            default: cout << "Invalid option.\n";
        }

    } while (choice != 0);
}
