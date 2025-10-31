#ifndef SUPPLY_VALIDATION_HPP
#define SUPPLY_VALIDATION_HPP

#include <string>
#include <iostream>

/**
 * @brief Contains validation functions specific to medical supplies
 *
 * @details
 * Used to verify user input and dataset fields before pushing to the stack
 */
namespace SupplyValidation {
    /**
     * @brief Checks if a given supply type is valid
     * @param type The string representing the supply type (MED, EQP, PPE)
     * @return true if valid, false otherwise
     */
    bool isValidSupplyType(const std::string& type);

    /**
     * @brief Validates quantity against capacity limits
     * @param quantity Current available quantity
     * @param minReq Minimum required stock
     * @param maxCap Maximum allowed stock
     * @return true if valid, false otherwise
     */
    bool isValidQuantity(int quantity, int minReq, int maxCap);

    /**
     * @brief Checks if a give date string is in "YYYY-MM-DD" format
     * @param date The date string to validate
     * @return true if format is valid, false otherwise
     */
    bool isValidDateFormat(const std::string& date);

    /**
     * @brief Determines if expiry date is still in the future
     * @param expiryDate Date string (YYYY-MM-DD)
     * @return true if future date, false if expired
     */
    bool isFutureDate(const std::string& expiryDate);
}

#endif