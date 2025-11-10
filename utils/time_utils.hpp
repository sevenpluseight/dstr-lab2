#ifndef TIME_UTILS_HPP
#define TIME_UTILS_HPP

#include <string>
#include <ctime> // Required for std::time_t

/**
 * @brief Get the current system timestamp formatted as "YYYY-MM-DD HH:MM:SS"
 */
std::string getCurrentTimestamp();

/**
 * @brief Get the current system time as a string, mimicking the ctime() format.
 * @param t The time_t object to format.
 * @return A string representing the formatted time (e.g., "Www Mmm dd hh:mm:ss yyyy").
 */
std::string getCurrentTimeString(std::time_t t);

#endif // TIME_UTILS_HPP
