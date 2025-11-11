#ifndef TIME_UTILS_HPP
#define TIME_UTILS_HPP

#include <string>
#include <ctime> // Required for std::time_t

/**
 * @brief Get the current system timestamp formatted as "YYYY-MM-DD HH:MM:SS"
 */
std::string getCurrentTimestamp();

/**
 * @brief Get the current system time as a string, mimicking the ctime() format
 * @param t The time_t object to format
 * @return A string representing the formatted time (e.g., "Www Mmm dd hh:mm:ss yyyy")
 */
std::string getCurrentTimeString(std::time_t t);

/**
 * @brief Checks if a given expiry date (YYYY-MM-DD) is before the current date.
 * @param expiryDate The expiry date string in "YYYY-MM-DD" format.
 * @return True if the expiry date is before the current date, false otherwise.
 */
bool isDateExpired(const std::string& expiryDate);

/**
 * @brief Checks if a given expiry date (YYYY-MM-DD) is within a specified number of days from the current date.
 * @param expiryDate The expiry date string in "YYYY-MM-DD" format.
 * @param days The number of days to consider for "expiring soon".
 * @return True if the expiry date is within the specified number of days from the current date, false otherwise.
 */
bool isDateExpiringSoon(const std::string& expiryDate, int days);

#endif // TIME_UTILS_HPP
