#ifndef MEDICAL_SUPPLY_MANAGER_HPP
#define MEDICAL_SUPPLY_MANAGER_HPP

#include <iostream>
#include <filesystem>

#include "entities.hpp"
#include "stack.hpp"

namespace fs = std::filesystem;

/**
 * @brief Handles medical supply management operations using a stack data structure
 *
 * @details
 * - Provides high-level functions for adding, removing and viewing supplies
 * - Acts as an interface between the user and the SupplyStack
 * - Uses stack operations (push, pop, peek) internally
 */
class MedicalSupplyManager {
private:
    SupplyStack stack; /// Stack for storing supply records

    /**
     * @brief Returns a full and safe path to a data file in the "data" folder
     *
     * @param filename Name of the file
     * @return std::string Full path to the file
     */
    std::string getDataFilePath(const std::string& filename) const {
        fs::path currentPath = fs::current_path();
        fs::path parentDir = currentPath.parent_path();
        fs::path dataDir = parentDir / "data";

        if (!fs::exists(dataDir)) {
            fs::create_directories(dataDir);
        }

        fs::path filePath = dataDir / filename;
        return filePath.string();
    }

public:
    /**
     * @brief Constructor initializes the manager with an empty stack
     */
    MedicalSupplyManager();

    /**
     * @brief Loads supplies from a medical_supply.csv into the stack
     * @param filename Path to the CSV file
     * @details Reads each line, parses the fields creates a Supply object and pushes it onto the internal stack
     */
    void loadFromCSV(const std::string& filename);

    /**
     * @brief Writes new supply into medical_supply.csv
     */
    void writeSupplyIntoCSV(const Supply& supply, const std::string& filename);

    /**
     * @brief Saves the current state of the stack to a CSV file
     * @param filename Path to the CSV file
     */
    void saveToCSV(const std::string& filename);

    /**
     * @brief Appends a removed supply record into removed_supplies.csv
     *
     * @param supply The supply that was removed
     */
    void writeRemovedSupply(const Supply& supply);

    /**
     * @brief Adds a new supply record to the stacks
     * @details Simulates recording a new stock entry
     */
    void addSupply();

    /**
     * @brief Uses (removes) the most recently added supply
     * @details Implements LIFO behavior
     */
    void useLastAddedSupply();

    /**
     * @brief Displays all current supplies in the stack
     */
    void viewCurrentSupply();

    /**
     * @brief Displays a user menu for interacting with the system
     */
    void displayMenu();
};

#endif
