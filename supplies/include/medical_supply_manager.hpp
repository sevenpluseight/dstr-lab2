#ifndef MEDICAL_SUPPLY_MANAGER_HPP
#define MEDICAL_SUPPLY_MANAGER_HPP

#include <iostream>

#include "entities.hpp"
#include "stack.hpp"

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
     * @brief Adds a new supply record to the stacks
     * @details Simulates recording a new stock entry
     */
    void addSupply();

    /**
     * @brief Uses (removes) the most recently added supply
     * @details Implements LIFO behavior
     * @note Pop operation conditions;
     *  1. Quantity == 0 (Out of stock)
     *  2. Expiry date < today (Mark as expired and remove)
     *
     *  Status != "Available" (Won't be removed but will mark as Low Stock)
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