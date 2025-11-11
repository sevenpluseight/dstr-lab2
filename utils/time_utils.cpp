#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "time_utils.hpp"

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm{};
#ifdef _WIN32
    localtime_s(&local_tm, &time_now);
#else
    localtime_r(&time_now, &local_tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string getCurrentTimeString(std::time_t t) {
    std::tm tmStruct;

#ifdef _WIN32
    localtime_s(&tmStruct, &t);
#else
    localtime_r(&t, &tmStruct);
#endif

    std::stringstream ss;
    ss << std::put_time(&tmStruct, "%a %b %d %H:%M:%S %Y");
    return ss.str();
}

bool isDateExpired(const std::string& expiryDate) {
    std::tm expiry_tm = {};
    std::istringstream ss(expiryDate);
    ss >> std::get_time(&expiry_tm, "%Y-%m-%d");

    if (ss.fail()) {
        return false; 
    }

    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    std::tm current_tm = {};
#ifdef _WIN32
    localtime_s(&current_tm, &current_time);
#else
    localtime_r(&current_time, &current_tm);
#endif

    if (expiry_tm.tm_year < current_tm.tm_year) return true;
    if (expiry_tm.tm_year > current_tm.tm_year) return false;

    if (expiry_tm.tm_mon < current_tm.tm_mon) return true;
    if (expiry_tm.tm_mon > current_tm.tm_mon) return false;

    if (expiry_tm.tm_mday < current_tm.tm_mday) return true;
    return false;
}

bool isDateExpiringSoon(const std::string& expiryDate, int days) {
    std::tm expiry_tm = {};
    std::istringstream ss(expiryDate);
    ss >> std::get_time(&expiry_tm, "%Y-%m-%d");

    if (ss.fail()) {
        return false;
    }

    std::time_t expiry_time = std::mktime(&expiry_tm);

    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    // Calculate the difference in seconds
    double difference_seconds = std::difftime(expiry_time, current_time);

    // Convert seconds to days
    int difference_days = static_cast<int>(difference_seconds / (60 * 60 * 24));

    // Check if the expiry date is in the future and within the specified number of days
    return difference_days >= 0 && difference_days <= days;
}
