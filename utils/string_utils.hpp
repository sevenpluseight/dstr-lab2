#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <cctype>

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

#endif
