#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "medical_supply_manager.hpp"
#include "message_handler.hpp"

/**
 * @brief Constructor
 */
MedicalSupplyManager::MedicalSupplyManager() {
    // Nothing needed
}

/**
 * @brief Displays a user menu for interacting with the medical supply system
 *
 * @details
 * Provides options to add, view or remove supplies from the stack
 * THe menu runs in a loop until the user selects "Exit"
 */
void MedicalSupplyManager::displayMenu() {
    int choice;

    // Load dataset
    loadFromCSV("medical_supply.csv");

    do {
        std::cout << "\n--------------- MEDICAL SUPPLY MANAGEMENT MENU -----------------\n";
        std::cout << "1. Add New Supply\n";
        std::cout << "2. Use (Remove) Last Added Supply\n";
        std::cout << "3. View Current Supplies Inventory\n";
        std::cout << "4. Exit\n";
        std::cout << "----------------------------------------------------------------\n";
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
                addSupply();
                break;
            case 2:
                useLastAddedSupply();
                break;
            case 3:
                viewCurrentSupply();
                break;
            case 4:
                MessageHandler::info("Exiting Medical Supply Management System...\n");
                break;
            default:
                MessageHandler::error("Invalid choice. Please select a valid option.");
                break;
        }
    } while (choice != 4);
}
