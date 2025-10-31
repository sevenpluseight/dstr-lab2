#include "session_manager.hpp"

int main() {
    std::string credentials_path = "data/user_credentials.csv";
    SessionManager::start(credentials_path);
    return 0;
}
