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
        file << "Supply_Batch_ID,Name,Supply_Type,Quantity,Min_Required,Max_Capacity,"
                "Status,Supplier_Name,Timestamp_Added,Expiry_Date\n";
    }

    // Write new supply entry
    file << supply.supply_batch_id << ","
         << supply.name << ","
         << supply.supply_type << ","
         << supply.quantity << ","
         << supply.min_required << ","
         << supply.max_capacity << ","
         << supply.status << ","
         << supply.supplier_name << ","
         << supply.timestamp_added << ","
         << supply.expiry_date << "\n" << std::flush;

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
    const int minReq = 150;
    int quantity, maxCap;
    std::string batchNumber, name, type, supplier, received, expiry, status;

    // Prompt for supply type
    type = getValidatedInput(
        "Enter supply type (MED/EQP/PPE): ",
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
    for (auto &c : shortName) c = toupper(c);
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
            try { return std::stoi(input) > 0; }
            catch (...) { return false; }
        },
        "Quantity must be a positive number."
    ));

    // Prompt for max capacity
    maxCap = std::stoi(getValidatedInput(
        "Enter maximum required: ",
        [quantity](const std::string& input) {
            try {
                int val = std::stoi(input);
                return SupplyValidation::isValidQuantity(quantity, 150, val);
            } catch (...) { return false; }
        },
        "Maximum must be at least 150 and not less than quantity."
        // It must be at least the minimum required stock - 150
        // It must be greater than or equal to the quantity that the user is trying to add
    ));

    // Prompt for supplier
    supplier = getValidatedInput(
        "Enter supplier: ",
        [](const std::string& input) { return !input.empty(); },
        "Supplier cannot be empty."
    );

    // Prompt for received date
    received = getValidatedInput(
        "Enter received date (YYYY-MM-DD): ",
        [](const std::string& input) { return SupplyValidation::isValidDateFormat(input); },
        "Invalid date format. Use YYYY-MM-DD."
    );


    // Prompt for expiry date
    expiry = getValidatedInput(
        "Enter expiry date (YYYY-MM-DD): ",
        [](const std::string& input) { return SupplyValidation::isValidDateFormat(input) && SupplyValidation::isFutureDate(input); },
        "Expiry date must be in the future and in YYYY-MM-DD format."
    );

    // Determine status
    status = SupplyValidation::isFutureDate(expiry) ? "Available" : "Expired";

    // Create supply and push to stack
    Supply s {supply_batch_id, name, type, quantity, minReq, maxCap, status, supplier, received, expiry};
    stack.push(s);

    writeSupplyIntoCSV(s, "medical_supply.csv");
}
