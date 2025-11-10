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
    localtime_s(&tmStruct, &t);  // Windows thread-safe
#else
    localtime_r(&t, &tmStruct);  // POSIX thread-safe
#endif

    std::stringstream ss;
    ss << std::put_time(&tmStruct, "%a %b %d %H:%M:%S %Y"); // mimic ctime() format
    return ss.str();
}
