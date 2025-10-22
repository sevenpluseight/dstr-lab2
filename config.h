#ifndef CONFIG_H
#define CONFIG_H

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

#endif // CONFIG_H
