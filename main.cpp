#include <iostream>
#include <filesystem>
#include "session_manager.hpp"

namespace fs = std::filesystem;

int main() {
    std::string credentials_path = "user_credentials.csv";
    SessionManager::start(credentials_path);
    return 0;
}
