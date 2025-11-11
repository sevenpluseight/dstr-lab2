#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>

#include "medical_supply_manager.hpp"
#include "supply_validation.hpp"
#include "message_handler.hpp"
#include "string_utils.hpp"
#include "stack.hpp"
#include "time_utils.hpp"

/**
 * @brief Writes the most recently added supply into medical_supply.csv
 *
 * @details
 * Appends the latest supply from the stack into the dataset file
 *
 * @param supply The supply object to be written into the file
 */
void MedicalSupplyManager::writeSupplyIntoCSV(const Supply& supply, const std::string& filename) {
    std::string filePath = getDataFilePath(filename);

    bool hasHeader = false;

    // Check if the header already exists
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
        MessageHandler::error("Failed to open " + filePath + " for writing.");
        return;
    }

    // Write header if missing
    if (!hasHeader) {
        file << "Supply_Batch_ID,Name,Supply_Type,Quantity,"
                "Status,Supplier_Name,Timestamp_Added,Expiry_Date\n";
    }

    // Write new supply entry
    file << supply.supply_batch_id << ","
         << supply.name << ","
         << supply.supply_type << ","
         << supply.quantity << ","
         << supply.status << ","
         << supply.supplier_name << ","
         << supply.timestamp_added << ","
         << supply.expiry_date << std::endl;

    if (file.fail()) {
        MessageHandler::error("Failed to write to " + filePath);
    }

    file.close();
    MessageHandler::info("New supply has been saved in the system.");
}

/**
 * @brief Adds a new supply to the stack after validating
 */
void MedicalSupplyManager::addSupply() {
    int quantity;
    std::string batchNumber, name, type, supplier, received, expiry, status;

    // Prompt for supply type
    type = getValidatedInput(
        "\nEnter supply type (MED/EQP/PPE): ",
        [](const std::string& input) { return SupplyValidation::isValidSupplyType(input); },
        "Invalid supply type. Must be MED, EQP or PPE."
    );

    // Prompt for supply name
    name = getValidatedInput(
        "Enter supply name: ",
        [](const std::string& input) { return !input.empty(); },
        "Supply name cannot be empty."
    );

    // Prompt for 4-digit batch number
    batchNumber = getValidatedInput(
        "Enter 4-digit batch number: ",
        [](const std::string& input) {
            if (input.size() != 4) return false;
            return std::all_of(input.begin(), input.end(), ::isdigit);
        },
        "Invalid batch number. Must be a 4-digit number (e.g., 1023)."
    );

    // Construct batch ID
    std::string shortName = name.substr(0, std::min<size_t>(3, name.size()));
    // for (auto &c : shortName) c = toupper(c);
    for (auto &c : shortName) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    std::string supply_batch_id = type + "-" + shortName + "-" + batchNumber;

    // Check for duplicate supply in the same batch
    if (SupplyValidation::isDuplicateSupply(stack, supply_batch_id, name)) {
        MessageHandler::error("Duplicate supply in the same batch is not allowed.");
        return;
    }

    // Prompt for quantity
    quantity = std::stoi(getValidatedInput(
        "Enter quantity: ",
        [](const std::string& input) {
            try { return std::stoi(input) >= 300; }
            catch (...) { return false; }
        },
        "Quantity must be a number greater than or equal to 300."
    ));

    // Prompt for supplier
    supplier = getValidatedInput(
        "Enter supplier: ",
        [](const std::string& input) { return !input.empty(); },
        "Supplier cannot be empty."
    );

    // Automatically set received date and time
    received = getCurrentTimestamp();

    // Calculate expiry date based on supply type
    // MED - 3 years
    // EQP - 7 years
    // PPE - 1 year
    int year, month, day;
    sscanf(received.substr(0, 10).c_str(), "%d-%d-%d", &year, &month, &day);

    if (type == "MED") {
        year += 3;
    } else if (type == "EQP") {
        year += 7;
    } else if (type == "PPE") {
        year += 1;
    }

    char expiryBuffer[11];
    snprintf(expiryBuffer, sizeof(expiryBuffer), "%04d-%02d-%02d", year, month, day);
    expiry = expiryBuffer;

    // Determine status
    status = SupplyValidation::isFutureDate(expiry) ? "Available" : "Expired";

    // Create supply and push to stack
    Supply s {supply_batch_id, name, type, quantity, status, supplier, received, expiry};
    stack.push(s);

    writeSupplyIntoCSV(s, "medical_supply.csv");
}
