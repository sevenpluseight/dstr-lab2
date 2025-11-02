#ifndef CONFIG_HPP
#define CONFIG_HPP

/**
 * @brief Platform and compiler detection header for cross-platform compatibility
 *
 * Defines macros for detecting the OS (Windows, macOS, Linux) and compiler (MSVC,
 * GCC, Clang)
 *
 * @note
 * Do NOT modify or rename the macros in this file unless you fully understand their dependencies
 *
 * If you change these definitions:
 *      - Colored terminal output in `message_handler.hpp` may break
 *      - Cross-platform compilation or runtime behavior may become unstable
 *
 * Any platform or compiler adjustments should be done carefully and tested across all supported environments
 *
 * ℹ️ If you modify or encounter issues related to OS or compiler macros, please raise it in the WhatsApp group before making changes
 */

// Detect OS platform
#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
    #define OS_MAC
#elif defined(__linux__)
    #define OS_LINUX
#else
    #define OS_UNKNOWN
#endif

// Detect compiler
#if defined(_MSC_VER)
    #define COMPILER_MSVC
#elif defined(__clang__)
    #define COMPILER_CLANG
#elif defined(__GNUC__)
    #define COMPILER_GCC
#endif

// Common macros for platform-specific code
#ifdef OS_WINDOWS
    #define CLEAR_COMMAND "cls"
#else
    #define CLEAR_COMMAND "clear"
#endif

#endif
