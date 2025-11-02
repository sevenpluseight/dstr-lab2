#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <cctype>
#include <functional>

#include "message_handler.hpp"

/**
 * @brief Trim whitespace (spaces, tabs, newlines, carriage returns) from both ends of a string
 * @param str The input string
 * @return Trimmed string
 */
inline std::string trim(const std::string &str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end   = str.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

/**
 * @brief Prompts the user for input and validates it using a provided validator function
 *
 * @details
 * This function repeatedly prompts the user until valid input is provided.
 * It uses a validator function to check the input and prints a message if the input is invalid.
 * Can be reused for any role
 *
 * @param prompt The message shown to the user before input
 * @param validator A function that takes a string and returns tru if input is valid
 * @param errorMsg The message to display when input fails validation
 * @return The valid input entered by the user
 *
 * @note
 * Anyone using this function must create their own validation function (can refer to supplies/include/supply_validation.hpp)
 * This function is generic and reusable, so it helps avoid repeating input-checking
 * code in multiple places (keeping the code cleaner and easier to maintain)
 */
inline std::string getValidatedInput(const std::string& prompt, std::function<bool(const std::string&)> validator, const std::string& errorMsg) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        if (validator(input)) {
            return input;
        } else {
            MessageHandler::error(errorMsg);
        }
    }
}

#endif
