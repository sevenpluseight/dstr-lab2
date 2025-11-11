#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "medical_supply_manager.hpp"
#include "message_handler.hpp"
#include "string_utils.hpp"
#include "stack.hpp"
#include "path_utils.hpp"

/**
* @brief Loads CSV file
 */
void MedicalSupplyManager::loadFromCSV(const std::string& filename) {
    std::string filePath = getDataFilePath(filename);

    std::ifstream file(filePath);
    if (!file.is_open()) {
        MessageHandler::error("Unable to open dataset: " + filePath);
        return;
    }

    std::string line;
    std::getline(file, line); // Skip CSV header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string supply_batch_id, name, type, status, supplier, received, expiry;
        int quantity;

        if (!std::getline(ss, supply_batch_id, ',')) continue;
        if (!std::getline(ss, name, ',')) continue;
        if (!std::getline(ss, type, ',')) continue;
        std::string quantityStr;
        if (!std::getline(ss, quantityStr, ',')) continue;
        // if (!std::getline(ss, maxCapStr, ',')) continue;
        if (!std::getline(ss, status, ',')) continue;
        if (!std::getline(ss, supplier, ',')) continue;
        if (!std::getline(ss, received, ',')) continue;
        if (!std::getline(ss, expiry, ',')) continue;

        quantity = std::stoi(trim(quantityStr));
        // maxCap   = std::stoi(trim(maxCapStr));

        Supply s{
            trim(supply_batch_id),
            trim(name),
            trim(type),
            quantity,
            // maxCap,
            trim(status),
            trim(supplier),
            trim(received),
            trim(expiry)
        };

        stack.push(s);
    }

    file.close();
}

void MedicalSupplyManager::saveToCSV(const std::string &filename) {
    std::string filePath = getDataFilePath(filename);
    std::ofstream file(filePath, std::ios::trunc); // Overwrite the file

    if (!file.is_open()) {
        MessageHandler::error("Failed to open " + filePath + " for writing.");
        return;
    }

    // Write header
    file << "Supply_Batch_ID,Name,Supply_Type,Quantity,"
            "Status,Supplier_Name,Timestamp_Added,Expiry_Date\n";

    // Write all supplies from the stack
    for (int i = 0; i <= stack.getTopIndex(); ++i) {
        Supply s = stack.getSupplyAt(i);
        file << s.supply_batch_id << ","
             << s.name << ","
             << s.supply_type << ","
             << s.quantity << ","
             // << s.max_capacity << ","
             << s.status << ","
             << s.supplier_name << ","
             << s.timestamp_added << ","
             << s.expiry_date << "\n";
    }

    file.close();
}

void MedicalSupplyManager::loadSupplyUsageLog() {
    std::string filePath = getDataFilePath("supply_usage_log.csv");

    std::ifstream file(filePath);
    if (!file.is_open()) {
        MessageHandler::error("Unable to open dataset: " + filePath);
        return;
    }

    std::string line;
    std::getline(file, line); // Skip CSV header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string case_id, patient_id, supply_batch_id, supply_name, quantity_used_str, status;
        
        if (!std::getline(ss, case_id, ',')) continue;
        if (!std::getline(ss, patient_id, ',')) continue;
        if (!std::getline(ss, supply_batch_id, ',')) continue;
        if (!std::getline(ss, supply_name, ',')) continue;
        if (!std::getline(ss, quantity_used_str, ',')) continue;
        if (!std::getline(ss, status, ',')) continue;

        SupplyUsageLog log {
            trim(case_id),
            trim(patient_id),
            trim(supply_batch_id),
            trim(supply_name),
            std::stoi(trim(quantity_used_str)),
            trim(status)
        };

        usage_logs.appendArray(log);
    }

    file.close();
}

void MedicalSupplyManager::saveSupplyUsageLog() {
    std::string filePath = getDataFilePath("supply_usage_log.csv");
    std::ofstream file(filePath, std::ios::trunc); // Overwrite the file

    if (!file.is_open()) {
        MessageHandler::error("Failed to open " + filePath + " for writing.");
        return;
    }

    // Write header
    file << "Case_ID,Patient_ID,Supply_Batch_ID,Supply_Name,Quantity_Used,Status\n";

    // Write all logs from the dynamic array
    for (int i = 0; i < usage_logs.getSize(); ++i) {
        SupplyUsageLog log = usage_logs.getElementAt(i);
        file << log.case_id << ","
             << log.patient_id << ","
             << log.supply_batch_id << ","
             << log.supply_name << ","
             << log.quantity_used << ","
             << log.status << "\n";
    }

    file.close();
}
