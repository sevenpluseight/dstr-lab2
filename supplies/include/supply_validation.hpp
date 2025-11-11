#ifndef SUPPLY_VALIDATION_HPP
#define SUPPLY_VALIDATION_HPP

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include <string>
#include <iostream>
#include <regex>
#include <ctime>
#include <sstream>

#include "stack.hpp"

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
    bool isValidSupplyType(const std::string& type) {
        return type == "MED" || type == "EQP" || type == "PPE";
    }

    /**
     * @brief Checks if a give date string is in "YYYY-MM-DD" format
     * @param date The date string to validate
     * @return true if format is valid, false otherwise
     */
    inline bool isValidDateFormat(const std::string& date) {
        std::regex pattern(R"(\d{4}-\d{2}-\d{2})");
        return std::regex_match(date, pattern);
    }

    /**
     * @brief Determines if expiry date is still in the future
     * @param expiryDate Date string (YYYY-MM-DD)
     * @return true if future date, false if expired
     */
    inline bool isFutureDate(const std::string& expiryDate) {
        // Get current system date
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);

        int year, month, day;
        std::sscanf(expiryDate.c_str(), "%d-%d-%d", &year, &month, &day);

        if (year > (now->tm_year + 1900)) return true;
        if (year < (now->tm_year + 1900)) return false;
        if (month > (now->tm_mon + 1)) return true;
        if (month < (now->tm_mon + 1)) return false;
        return day > now->tm_mday;
    }

    /**
     * @brief Check if a supply with the same supplyBatchID and supply name already exists
     * @param stack The current SupplyStack
     * @param supplyBatchID Supply Batch ID entered by the user
     * @param supplyName Supply name entered by the user
     * @return true if duplicate exists, false otherwise
     */
    inline bool isDuplicateSupply(const SupplyStack& stack, const std::string& supplyBatchID, const std::string& supplyName) {
        for (int i = stack.getTopIndex(); i >= 0; i--) {
            Supply s = stack.getSupplyAt(i);
            if (s.supply_batch_id == supplyBatchID && s.name == supplyName) {
                return true; // Duplicate found
            }
        }
        return false; // No duplicate
    }
}

#endif
