#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <cctype>
#include <functional>
#include "algorithm"
#include <sstream>

#include "message_handler.hpp"
#include "dynamic_array.hpp"

/**
 * @brief Common string helper and input validation utilities used across hospital system modules
 *
 * @note
 * - This header provides reusable helper functions for string processing and input validation
 * - It depends on `message_handler.hpp` for displaying colored error messages when validation fails
 * - Do NOT modify or specialize these functions directly for specific modules, instead, write separate
 *   validator functions (e.g., supply_validation.hpp) and reuse `getValidatedInput`
 * - Any change in core string handling logic here affects ALL modules that rely on user input or text formatting
 *
 * @usage
 * - Include in any module that needs:
 *      1. Whitespace trimming for user input or CSV parsing
 *      2. Interactive input validation with clear feedback
 * - Example:
 *      #include "string_utils.hpp"
 *
 *      bool isValidName(const std::string& input) {
 *          return !input.empty() && input.size() > 2;
 *      }
 *
 *      std::string name = getValidatedInput("Enter name: ", isValidName, "Invalid name. Try again.");
 *
 * ℹ️ If you encounter logic or validation issues, please raise it in the WhatsApp group before making changes
 */

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
 * @brief Convert all characters in a string to uppercase
 *
 * @details
 * This function uses std::transform and std::toupper to convert each character
 * in the given string to its uppercase equivalent. It operates directly on the
 * provided string and returns a new uppercase version
 *
 * @param s The input string to convert
 * @return A new string with all characters converted to uppercase
 *
 * @note
 * Non-alphabetic characters are not affected. Works with ASCII characters
 *
 * @example
 * std::string name = "John Doe";
 * std::string upper = toUpper(name); // "JOHN DOE"
 */
inline std::string toUpper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return static_cast<char>(std::toupper(c)); }
                  );
    return s;
}


/**
 * @brief Split a string into substrings using a delimiter and store them in a DynamicArray
 *
 * @details
 * This function separates the input text based on the specified delimiter character
 * Each extracted substring is trimmed of leading and trailing whitespace using the
 * `trim()` helper function, then appended to the provided DynamicArray.
 *
 * @param text The input string to split
 * @param delimeter The character used to separate tokens in the input string
 * @param arr The DynamicArray to append each trimmed token into
 * @return A DynamicArray containing all split and trimmed substrings
 *
 * @note
 * - The provided DynamicArray is returned after all tokens are appended
 * - Empty tokens (e.g., consecutive delimiters) are also added as empty strings
 *
 * @example
 * DynamicArray arr;
 * arr = split("apple, banana, cherry", ',', arr);
 * // Result: ["apple", "banana", "cherry"]
 */
inline DynamicArray split(std::string text, char delimeter, DynamicArray arr) {
    std::stringstream rawStream(text);
    std::string word = "";
    while(getline(rawStream, word, delimeter)) {
        arr.appendArray(trim(word));
    }
    return arr;
};

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
