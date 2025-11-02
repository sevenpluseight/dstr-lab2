#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "medical_supply_manager.hpp"
#include "message_handler.hpp"
#include "string_utils.hpp"
#include "stack.hpp"

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
        int quantity, minReq, maxCap;

        if (!std::getline(ss, supply_batch_id, ',')) continue;
        if (!std::getline(ss, name, ',')) continue;
        if (!std::getline(ss, type, ',')) continue;
        std::string quantityStr, minReqStr, maxCapStr;
        if (!std::getline(ss, quantityStr, ',')) continue;
        if (!std::getline(ss, minReqStr, ',')) continue;
        if (!std::getline(ss, maxCapStr, ',')) continue;
        if (!std::getline(ss, status, ',')) continue;
        if (!std::getline(ss, supplier, ',')) continue;
        if (!std::getline(ss, received, ',')) continue;
        if (!std::getline(ss, expiry, ',')) continue;

        quantity = std::stoi(trim(quantityStr));
        minReq   = std::stoi(trim(minReqStr));
        maxCap   = std::stoi(trim(maxCapStr));

        Supply s{
            trim(supply_batch_id),
            trim(name),
            trim(type),
            quantity,
            minReq,
            maxCap,
            trim(status),
            trim(supplier),
            trim(received),
            trim(expiry)
        };

        stack.push(s);
    }

    file.close();
}
