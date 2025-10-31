#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <iostream>
#include <string>
#include "config.hpp" // Required for OS detection macros

/**
 * @brief Message handling utility for all roles
 */
class MessageHandler {
public:
    /**
     * @brief Display an informational message (green)
     */
    static void info(const std::string& msg) {
#ifdef OS_WINDOWS
        std::cout << msg << std::endl;
#else
        std::cout << "\033[32m" << msg << "\033[0m" << std::endl;
#endif
    }

    /**
     * @brief Display a warning message (yellow)
     */
    static void warning(const std::string& msg) {
#ifdef OS_WINDOWS
        std::cout << msg << std::endl;
#else
        std::cout << "\033[33m" << msg << "\033[0m" << std::endl;
#endif
    }

    /**
     * @brief Display an error message (red)
     */
    static void error(const std::string& msg) {
#ifdef OS_WINDOWS
        std::cout << msg << std::endl;
#else
        std::cout << "\033[31m" << msg << "\033[0m" << std::endl;
#endif
    }

    /**
     * @brief Display a custom message (cyan)
     */
    static void custom(const std::string& msg) {
#ifdef OS_WINDOWS
        std::cout << msg << std::endl;
#else
        std::cout << "\033[36m" << msg << "\033[0m" << std::endl;
#endif
    }
};

#endif // MESSAGE_HANDLER_HPP
