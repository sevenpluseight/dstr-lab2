#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <iostream>
#include <string>
#include "config.hpp" // Required for OS detection macros (used in color handling)

/**
 * @brief Cross-platform message handling utility for all program roles
 *
 * @details
 * Provides a unified way to display:
 * - Informational messages (green)
 * - Warnings (yellow)
 * - Errors (red)
 * - Custom-labeled messages (cyan)
 *
 * Supports colored console messages (auto-disabled on Windows if ANSI unsupported)
 *
 * @usage
 * - MessageHandler::info("message");
 * - MessageHandler::warning("message");
 * - MessageHandler::error("message");
 * - MessageHandler::custom("[SUCCESS]", "Operation completed");
 */
class MessageHandler {
public:
    /**
     * @brief Display an informational message (green)
     */
    static void info(const std::string& msg) {
#ifdef OS_WINDOWS
        std::cout << "[INFO] " << msg << std::endl;
#else
        std::cout << "\033[32m[INFO]\033[0m " << msg << std::endl;
#endif
    }

    /**
     * @brief Display a warning message (yellow)
     */
    static void warning(const std::string& msg) {
#ifdef OS_WINDOWS
        std::cout << "[WARNING] " << msg << std::endl;
#else
        std::cout << "\033[33m[WARNING]\033[0m " << msg << std::endl;
#endif
    }

    /**
     * @brief Display an error message (red)
     */
    static void error(const std::string& msg) {
#ifdef OS_WINDOWS
        std::cerr << "[ERROR] " << msg << std::endl;
#else
        std::cerr << "\033[31m[ERROR]\033[0m " << msg << std::endl;
#endif
    }

    /**
     * @brief Display a custom message (cyan) with a label
     * @param label Custom label (e.g., "[DEBUG]", "[SUCCESS]")
     * @param msg Message content
     */
    static void custom(const std::string& label, const std::string& msg) {
#ifdef OS_WINDOWS
        std::cout << label << " " << msg << std::endl;
#else
        std::cout << "\033[36m" << label << "\033[0m " << msg << std::endl;
#endif
    }
};

#endif // MESSAGE_HANDLER_HPP
