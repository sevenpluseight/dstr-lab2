#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "medical_supply_manager.hpp"
#include "message_handler.hpp"
#include "stack.hpp"
#include "time_utils.hpp"
#include "path_utils.hpp"

/**
 * @brief Displays inventory menu
 */
void MedicalSupplyManager::displayInventoryMenu() {
    int choice;
    do {
        std::cout << "\n--------------- VIEW SUPPLY INVENTORY --------------------\n";
        std::cout << "1. View All Supplies\n";
        std::cout << "2. View Available Supplies\n";
        std::cout << "3. View Expired Supplies\n";
        std::cout << "4. View Medicine Supplies\n";
        std::cout << "5. View Equipment Supplies\n";
        std::cout << "6. View PPE Supplies\n";
        std::cout << "7. View Supplies Expiring Soon\n";
        std::cout << "8. View Removed Supplies\n";
        std::cout << "9. Back to Main Menu\n";
        std::cout << "----------------------------------------------------------\n";
        std::cout << "Enter a number to perform an action: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            MessageHandler::error("Invalid input. Please enter a number.");
            continue;
        }

        std::cin.ignore();

        switch (choice) {
            case 1:
                viewAllSupply();
                break;
            case 2:
                viewAvailableSupply();
                break;
            case 3:
                viewExpiredSupply();
                break;
            case 4:
                viewMedicineSupply();
                break;
            case 5:
                viewEquipmentSupply();
                break;
            case 6:
                viewPPESupply();
                break;
            case 7:
                viewToBeExpiredSupply();
                break;
            case 8:
                viewRemovedSupplies();
                break;
            case 9:
                break;
            default:
                MessageHandler::error("Invalid choice. Please select a valid option.");
                break;
        }
    } while (choice != 9);
}

void MedicalSupplyManager::viewExpiredSupply() {
    if (stack.isStackEmpty()) {
        MessageHandler::warning("No supplies available to check for expiry.");
        return;
    }

    SupplyStack expiredStack;
    bool found = false;

    for (int i = 0; i <= stack.getTopIndex(); ++i) {
        Supply s = stack.getSupplyAt(i);
        if (isDateExpired(s.expiry_date)) {
            expiredStack.push(s);
            found = true;
        }
    }

    if (!found) {
        MessageHandler::info("No expired supplies found.");
        return;
    }

    std::cout << "\nEXPIRED SUPPLY INVENTORY\n";

    constexpr int wBatchID = 20, wName = 25, wType = 15, wQuantity = 10,
                  wStatus = 16, wSupplierName = 32, wTimestampAdded = 25, wExpiryDate = 18;

    // Table header
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate, '-') << "\n";
    std::cout << std::left
              << std::setw(wBatchID) << "Supply Batch ID"
              << std::setw(wName) << "Name"
              << std::setw(wType) << "Supply Type"
              << std::setw(wQuantity) << "Quantity"
              << std::setw(wStatus) << "Status"
              << std::setw(wSupplierName) << "Supplier Name"
              << std::setw(wTimestampAdded) << "Timestamp Added"
              << std::setw(wExpiryDate) << "Expiry Date" << "\n";
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate, '-') << "\n";

    // Print each expired supply
    for (int i = expiredStack.getTopIndex(); i >= 0; --i) {
        Supply s = expiredStack.getSupplyAt(i);
        std::cout << std::left
                  << std::setw(wBatchID) << s.supply_batch_id
                  << std::setw(wName) << s.name
                  << std::setw(wType) << s.supply_type
                  << std::setw(wQuantity) << s.quantity
                  << "\033[31m" << std::setw(wStatus) << "Expired" << "\033[0m"
                  << std::setw(wSupplierName) << s.supplier_name
                  << std::setw(wTimestampAdded) << s.timestamp_added
                  << std::setw(wExpiryDate) << s.expiry_date << "\n";
    }
}

void MedicalSupplyManager::viewAvailableSupply() {
    if (stack.isStackEmpty()) {
        MessageHandler::warning("No supplies available.");
        return;
    }

    SupplyStack availableStack;
    bool found = false;

    for (int i = 0; i <= stack.getTopIndex(); ++i) {
        Supply s = stack.getSupplyAt(i);
        if (s.status == "Available" && s.quantity > 0) {
            availableStack.push(s);
            found = true;
        }
    }

    if (!found) {
        MessageHandler::info("No available supplies found.");
        return;
    }

    std::cout << "\nAVAILABLE SUPPLY INVENTORY\n";

    constexpr int wBatchID = 20, wName = 25, wType = 15, wQuantity = 10,
                  wStatus = 16, wSupplierName = 32, wTimestampAdded = 25, wExpiryDate = 18;

    // Table header
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate, '-') << "\n";
    std::cout << std::left
              << std::setw(wBatchID) << "Supply Batch ID"
              << std::setw(wName) << "Name"
              << std::setw(wType) << "Supply Type"
              << std::setw(wQuantity) << "Quantity"
              << std::setw(wStatus) << "Status"
              << std::setw(wSupplierName) << "Supplier Name"
              << std::setw(wTimestampAdded) << "Timestamp Added"
              << std::setw(wExpiryDate) << "Expiry Date" << "\n";
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate, '-') << "\n";

    // Print each available supply
    for (int i = availableStack.getTopIndex(); i >= 0; --i) {
        Supply s = availableStack.getSupplyAt(i);
        std::cout << std::left
                  << std::setw(wBatchID) << s.supply_batch_id
                  << std::setw(wName) << s.name
                  << std::setw(wType) << s.supply_type
                  << std::setw(wQuantity) << s.quantity
                  << "\033[32m" << std::setw(wStatus) << "Available" << "\033[0m"
                  << std::setw(wSupplierName) << s.supplier_name
                  << std::setw(wTimestampAdded) << s.timestamp_added
                  << std::setw(wExpiryDate) << s.expiry_date << "\n";
    }
}

void MedicalSupplyManager::viewMedicineSupply() {
    if (stack.isStackEmpty()) {
        MessageHandler::warning("No supplies available.");
        return;
    }

    SupplyStack medicineStack;
    bool found = false;

    for (int i = 0; i <= stack.getTopIndex(); ++i) {
        Supply s = stack.getSupplyAt(i);
        if (s.supply_type == "MED") {
            medicineStack.push(s);
            found = true;
        }
    }

    if (!found) {
        MessageHandler::info("No medicine supplies found.");
        return;
    }

    std::cout << "\nMEDICINE SUPPLY INVENTORY\n";

    constexpr int wBatchID = 20, wName = 25, wType = 15, wQuantity = 10,
                  wStatus = 16, wSupplierName = 32, wTimestampAdded = 25, wExpiryDate = 18;

    // Table header
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate, '-') << "\n";
    std::cout << std::left
              << std::setw(wBatchID) << "Supply Batch ID"
              << std::setw(wName) << "Name"
              << std::setw(wType) << "Supply Type"
              << std::setw(wQuantity) << "Quantity"
              << std::setw(wStatus) << "Status"
              << std::setw(wSupplierName) << "Supplier Name"
              << std::setw(wTimestampAdded) << "Timestamp Added"
              << std::setw(wExpiryDate) << "Expiry Date" << "\n";
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate, '-') << "\n";

    // Print each medicine supply
    for (int i = medicineStack.getTopIndex(); i >= 0; --i) {
        Supply s = medicineStack.getSupplyAt(i);
        std::cout << std::left
                  << std::setw(wBatchID) << s.supply_batch_id
                  << std::setw(wName) << s.name
                  << std::setw(wType) << s.supply_type
                  << std::setw(wQuantity) << s.quantity;
        
        if (s.status == "Available") {
            std::cout << "\033[32m" << std::setw(wStatus) << s.status << "\033[0m"; // Green
        } else if (s.status == "Expired") {
            std::cout << "\033[31m" << std::setw(wStatus) << s.status << "\033[0m"; // Red
        } else {
            std::cout << std::setw(wStatus) << s.status; // No color
        }

        std::cout << std::setw(wSupplierName) << s.supplier_name
                  << std::setw(wTimestampAdded) << s.timestamp_added
                  << std::setw(wExpiryDate) << s.expiry_date << "\n";
    }
}

void MedicalSupplyManager::viewEquipmentSupply() {
    if (stack.isStackEmpty()) {
        MessageHandler::warning("No supplies available.");
        return;
    }

    SupplyStack equipmentStack;
    bool found = false;

    for (int i = 0; i <= stack.getTopIndex(); ++i) {
        Supply s = stack.getSupplyAt(i);
        if (s.supply_type == "EQP") {
            equipmentStack.push(s);
            found = true;
        }
    }

    if (!found) {
        MessageHandler::info("No equipment supplies found.");
        return;
    }

    std::cout << "\nEQUIPMENT SUPPLY INVENTORY\n";

    constexpr int wBatchID = 20, wName = 25, wType = 15, wQuantity = 10,
                  wStatus = 16, wSupplierName = 32, wTimestampAdded = 25, wExpiryDate = 18;

    // Table header
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate, '-') << "\n";
    std::cout << std::left
              << std::setw(wBatchID) << "Supply Batch ID"
              << std::setw(wName) << "Name"
              << std::setw(wType) << "Supply Type"
              << std::setw(wQuantity) << "Quantity"
              << std::setw(wStatus) << "Status"
              << std::setw(wSupplierName) << "Supplier Name"
              << std::setw(wTimestampAdded) << "Timestamp Added"
              << std::setw(wExpiryDate) << "Expiry Date" << "\n";
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate, '-') << "\n";

    // Print each equipment supply
    for (int i = equipmentStack.getTopIndex(); i >= 0; --i) {
        Supply s = equipmentStack.getSupplyAt(i);
        std::cout << std::left
                  << std::setw(wBatchID) << s.supply_batch_id
                  << std::setw(wName) << s.name
                  << std::setw(wType) << s.supply_type
                  << std::setw(wQuantity) << s.quantity;
        
        if (s.status == "Available") {
            std::cout << "\033[32m" << std::setw(wStatus) << s.status << "\033[0m"; // Green
        } else if (s.status == "Expired") {
            std::cout << "\033[31m" << std::setw(wStatus) << s.status << "\033[0m"; // Red
        } else {
            std::cout << std::setw(wStatus) << s.status; // No color
        }

        std::cout << std::setw(wSupplierName) << s.supplier_name
                  << std::setw(wTimestampAdded) << s.timestamp_added
                  << std::setw(wExpiryDate) << s.expiry_date << "\n";
    }
}

void MedicalSupplyManager::viewPPESupply() {
    if (stack.isStackEmpty()) {
        MessageHandler::warning("No supplies available.");
        return;
    }

    SupplyStack ppeStack;
    bool found = false;

    for (int i = 0; i <= stack.getTopIndex(); ++i) {
        Supply s = stack.getSupplyAt(i);
        if (s.supply_type == "PPE") {
            ppeStack.push(s);
            found = true;
        }
    }

    if (!found) {
        MessageHandler::info("No PPE supplies found.");
        return;
    }

    std::cout << "\nPPE SUPPLY INVENTORY\n";

    constexpr int wBatchID = 20, wName = 25, wType = 15, wQuantity = 10,
                  wStatus = 16, wSupplierName = 32, wTimestampAdded = 25, wExpiryDate = 18;

    // Table header
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate, '-') << "\n";
    std::cout << std::left
              << std::setw(wBatchID) << "Supply Batch ID"
              << std::setw(wName) << "Name"
              << std::setw(wType) << "Supply Type"
              << std::setw(wQuantity) << "Quantity"
              << std::setw(wStatus) << "Status"
              << std::setw(wSupplierName) << "Supplier Name"
              << std::setw(wTimestampAdded) << "Timestamp Added"
              << std::setw(wExpiryDate) << "Expiry Date" << "\n";
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate, '-') << "\n";

    // Print each PPE supply
    for (int i = ppeStack.getTopIndex(); i >= 0; --i) {
        Supply s = ppeStack.getSupplyAt(i);
        std::cout << std::left
                  << std::setw(wBatchID) << s.supply_batch_id
                  << std::setw(wName) << s.name
                  << std::setw(wType) << s.supply_type
                  << std::setw(wQuantity) << s.quantity;
        
        if (s.status == "Available") {
            std::cout << "\033[32m" << std::setw(wStatus) << s.status << "\033[0m"; // Green
        } else if (s.status == "Expired") {
            std::cout << "\033[31m" << std::setw(wStatus) << s.status << "\033[0m"; // Red
        } else {
            std::cout << std::setw(wStatus) << s.status; // No color
        }

        std::cout << std::setw(wSupplierName) << s.supplier_name
                  << std::setw(wTimestampAdded) << s.timestamp_added
                  << std::setw(wExpiryDate) << s.expiry_date << "\n";
    }
}

void MedicalSupplyManager::viewToBeExpiredSupply() {
    if (stack.isStackEmpty()) {
        MessageHandler::warning("No supplies available to check for expiring soon.");
        return;
    }

    SupplyStack expiringSoonStack;
    bool found = false;
    const int EXPIRING_SOON_DAYS = 30; // Define "soon" as within 30 days

    for (int i = 0; i <= stack.getTopIndex(); ++i) {
        Supply s = stack.getSupplyAt(i);
        if (!isDateExpired(s.expiry_date) && isDateExpiringSoon(s.expiry_date, EXPIRING_SOON_DAYS)) {
            expiringSoonStack.push(s);
            found = true;
        }
    }

    if (!found) {
        MessageHandler::info("No supplies expiring within the next " + std::to_string(EXPIRING_SOON_DAYS) + " days found.");
        return;
    }

    std::cout << "\nSUPPLIES EXPIRING SOON (within " << EXPIRING_SOON_DAYS << " days)\n";

    constexpr int wBatchID = 20, wName = 25, wType = 15, wQuantity = 10,
                  wStatus = 16, wSupplierName = 32, wTimestampAdded = 25, wExpiryDate = 18;

    // Table header
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate, '-') << "\n";
    std::cout << std::left
              << std::setw(wBatchID) << "Supply Batch ID"
              << std::setw(wName) << "Name"
              << std::setw(wType) << "Supply Type"
              << std::setw(wQuantity) << "Quantity"
              << std::setw(wStatus) << "Status"
              << std::setw(wSupplierName) << "Supplier Name"
              << std::setw(wTimestampAdded) << "Timestamp Added"
              << std::setw(wExpiryDate) << "Expiry Date" << "\n";
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate, '-') << "\n";

    // Print each expiring soon supply
    for (int i = expiringSoonStack.getTopIndex(); i >= 0; --i) {
        Supply s = expiringSoonStack.getSupplyAt(i);
        std::cout << std::left
                  << std::setw(wBatchID) << s.supply_batch_id
                  << std::setw(wName) << s.name
                  << std::setw(wType) << s.supply_type
                  << std::setw(wQuantity) << s.quantity
                  << "\033[33m" << std::setw(wStatus) << "Expiring Soon" << "\033[0m" // Yellow
                  << std::setw(wSupplierName) << s.supplier_name
                  << std::setw(wTimestampAdded) << s.timestamp_added
                  << std::setw(wExpiryDate) << s.expiry_date << "\n";
    }
}

/**
 * @brief Displays all supplies in the stack
 */
void MedicalSupplyManager::viewAllSupply() {
    if (stack.isStackEmpty()) {
        MessageHandler::warning("No supplies available.");
        return;
    }

    std::cout << "\nCURRENT SUPPLY INVENTORY\n";

    constexpr  int wBatchID = 20;
    constexpr int wName    = 25;
    constexpr int wType    = 15;
    constexpr int wQuantity= 10;
    // constexpr int wMaxCap  = 12;
    constexpr int wStatus  = 16;
    constexpr int wSupplierName= 32;
    constexpr int wTimestampAdded= 25;
    constexpr int wExpiryDate= 18;

    std::cout << std::string(
        wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate,
        '-') << "\n";

    // Print table header to match CSV header from add_supply.cpp
    std::cout << std::left
              << std::setw(wBatchID) << "Supply Batch ID"
              << std::setw(wName)    << "Name"
              << std::setw(wType)    << "Supply Type"
              << std::setw(wQuantity)<< "Quantity"
              // << std::setw(wMaxCap)  << "Max_Capacity"
              << std::setw(wStatus)  << "Status"
              << std::setw(wSupplierName)<< "Supplier Name"
              << std::setw(wTimestampAdded)<< "Timestamp Added"
              << std::setw(wExpiryDate)<< "Expiry Date"
              << "\n";

    // Print line separator
    std::cout << std::string(
        wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate,
        '-') << "\n";

    // Print each supply record from top (newest) to bottom (oldest)
    for (int i = stack.getTopIndex(); i >= 0; --i) {
        Supply s = stack.getSupplyAt(i);

        std::cout << std::left
                  << std::setw(wBatchID) << s.supply_batch_id
                  << std::setw(wName)    << s.name
                  << std::setw(wType)    << s.supply_type
                  << std::setw(wQuantity)<< s.quantity;
                  // << std::setw(wMaxCap)  << s.max_capacity;

        // Apply color based on status
        if (s.status == "Available") {
            std::cout << "\033[32m" << std::setw(wStatus) << s.status << "\033[0m"; // Green
        } else if (s.status == "Expired") {
            std::cout << "\033[31m" << std::setw(wStatus) << s.status << "\033[0m"; // Red
        } else {
            std::cout << std::setw(wStatus) << s.status; // No color
        }

        std::cout << std::setw(wSupplierName)   << s.supplier_name
                  << std::setw(wTimestampAdded) << s.timestamp_added
                  << std::setw(wExpiryDate)     << s.expiry_date
                  << "\n";
    }
}

void MedicalSupplyManager::viewRemovedSupplies() {
    std::string filename = getDataFilePath("removed_supplies.csv");
    std::ifstream file(filename);

    if (!file.is_open()) {
        MessageHandler::error("Could not open file: " + filename);
        return;
    }

    std::string line;
    // Skip header line
    if (!std::getline(file, line)) {
        MessageHandler::info("Removed supplies file is empty.");
        return;
    }

    std::cout << "\nREMOVED SUPPLY HISTORY\n";

    constexpr int wBatchID = 20, wName = 25, wType = 15, wQuantity = 10,
                  wStatus = 16, wSupplierName = 32, wTimestampAdded = 25, wExpiryDate = 18, wRemovedOn = 25;

    // Table header
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate + wRemovedOn, '-') << "\n";
    std::cout << std::left
              << std::setw(wBatchID) << "Supply Batch ID"
              << std::setw(wName) << "Name"
              << std::setw(wType) << "Supply Type"
              << std::setw(wQuantity) << "Quantity"
              << std::setw(wStatus) << "Status"
              << std::setw(wSupplierName) << "Supplier Name"
              << std::setw(wTimestampAdded) << "Timestamp Added"
              << std::setw(wExpiryDate) << "Expiry Date"
              << std::setw(wRemovedOn) << "Removed On" << "\n";
    std::cout << std::string(wBatchID + wName + wType + wQuantity + wStatus + wSupplierName + wTimestampAdded + wExpiryDate + wRemovedOn, '-') << "\n";

    bool found = false;
    while (std::getline(file, line)) {
        found = true;
        std::stringstream ss(line);
        std::string field;
        
        std::string supply_batch_id, name, supply_type, quantity, status, supplier_name, timestamp_added, expiry_date, removed_on;

        std::getline(ss, supply_batch_id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, supply_type, ',');
        std::getline(ss, quantity, ',');
        std::getline(ss, status, ',');
        std::getline(ss, supplier_name, ',');
        std::getline(ss, timestamp_added, ',');
        std::getline(ss, expiry_date, ',');
        std::getline(ss, removed_on, ',');

        std::cout << std::left
                  << std::setw(wBatchID) << supply_batch_id
                  << std::setw(wName) << name
                  << std::setw(wType) << supply_type
                  << std::setw(wQuantity) << quantity
                  << std::setw(wStatus) << status
                  << std::setw(wSupplierName) << supplier_name
                  << std::setw(wTimestampAdded) << timestamp_added
                  << std::setw(wExpiryDate) << expiry_date
                  << std::setw(wRemovedOn) << removed_on << "\n";
    }

    if (!found) {
        MessageHandler::info("No removed supplies found in the file.");
    }

    file.close();
}
