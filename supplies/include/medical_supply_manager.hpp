#ifndef MEDICAL_SUPPLY_MANAGER_HPP
#define MEDICAL_SUPPLY_MANAGER_HPP

#include <iostream>
#include <filesystem>

#include "entities.hpp"
#include "stack.hpp"
#include "message_handler.hpp"
#include "path_utils.hpp"

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
    void viewAllSupply();

    /**
     * @brief Displays all expired supplies in the stack
     */
    void viewExpiredSupply();

    /**
     * @brief Displays all available supplies in the stack
     */
    void viewAvailableSupply();

    /**
     * @brief Displays all medicine supplies in the stack
     */
    void viewMedicineSupply();

    /**
     * @brief Displays all equipment supplies in the stack
     */
    void viewEquipmentSupply();

    /**
     * @brief Displays all PPE supplies in the stack
     */
    void viewPPESupply();

    /**
     * @brief Displays all supplies that are about to expire in the stack
     */
    void viewToBeExpiredSupply();

    /**
     * @brief Displays all removed supplies
     */
    void viewRemovedSupplies();

    /*
     * @brief Displays a view inventory menu
     */
    void displayInventoryMenu();

    /**
     * @brief Displays a user menu for interacting with the system
     */
    void displayMenu();
};

#endif
