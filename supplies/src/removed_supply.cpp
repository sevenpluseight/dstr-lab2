#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "medical_supply_manager.hpp"
#include "message_handler.hpp"
#include "stack.hpp"

/**
* @brief Appends a removed supply record into removed_supplies.csv
 *
 * @param supply The supply that was removed
 */
void MedicalSupplyManager::writeRemovedSupply(const Supply& supply) {
    std::string filePath = getDataFilePath("removed_supplies.csv");

    bool hasHeader = false;

    // Check for existing header
    {
        std::ifstream check(filePath);
        if (check.is_open()) {
            std::string firstLine;
            if (std::getline(check, firstLine) &&
                firstLine.find("Supply_Batch_ID") != std::string::npos)
            {
                hasHeader = true;
            }
        }
    }

    std::ofstream file(filePath, std::ios::app);
    if (!file.is_open()) {
        MessageHandler::error("Failed to open " + filePath + " for writing removed supplies.");
        return;
    }

    // Write header if not present
    if (!hasHeader) {
        file << "Supply_Batch_ID,Name,Supply_Type,Quantity,Min_Required,Max_Capacity,"
                "Status,Supplier_Name,Timestamp_Added,Expiry_Date,Removed_On\n";
    }

    // Get current date for removal
    std::time_t now = std::time(nullptr);
    char dateStr[20];
    std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", std::localtime(&now));

    // Append supply details
    file << supply.supply_batch_id << ","
         << supply.name << ","
         << supply.supply_type << ","
         << supply.quantity << ","
         << supply.min_required << ","
         << supply.max_capacity << ","
         << supply.status << ","
         << supply.supplier_name << ","
         << supply.timestamp_added << ","
         << supply.expiry_date << ","
         << dateStr << "\n" << std::flush;

    if (file.fail()) {
        MessageHandler::error("Failed to write to " + filePath);
    }

    file.close();

    // MessageHandler::info("Removed supply has been recorded.");
}

/**
 * @brief Removes (uses) the most recently added supply from the stack
 */
void MedicalSupplyManager::useLastAddedSupply() {
    if (stack.isStackEmpty()) {
        MessageHandler::error("No supplies available to use.");
        return;
    }

    // Pop the most recent supply
    Supply removed = stack.pop();

    // Display removed supply details
    MessageHandler::info("\nThe most recently added supply has been removed:\n");
    std::cout << std::left
              << std::setw(20) << "Supply Batch ID"
              << std::setw(25) << "Supply Name"
              << std::setw(8)  << "Type"
              << std::setw(10) << "Quantity"
              << std::setw(12) << "Status" << "\n";
    std::cout << std::string(75, '-') << "\n";
    std::cout << std::left
              << std::setw(20) << removed.supply_batch_id
              << std::setw(25) << removed.name
              << std::setw(8)  << removed.supply_type
              << std::setw(10) << removed.quantity
              << std::setw(12) << removed.status << "\n";
    std::cout << std::string(75, '-') << "\n";

    // Save the updated stack to the CSV file
    saveToCSV("medical_supply.csv");

    // Log the removed supply
    writeRemovedSupply(removed);

    MessageHandler::info("Removed supply logged and inventory updated successfully.");
}
