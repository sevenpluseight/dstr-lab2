#include "session_manager.hpp"

/**
 * @brief Starts the login loop
 * @param credentials_path Path to user credentials file
 */
void SessionManager::start(const std::string& credentials_path) {
    while (true) {
        MessageHandler::info("\nHospital Management System");
        Login login(credentials_path);
        login.promptLogin();

        // When a role calls logout(), this loop continues. Otherwise, if program exists inside a role, it stops here
    }
}

/**
 * @brief Handles logout and re-login
 * @param credentials_path Path to user credentials file
 */
void SessionManager::logout(const std::string &credentials_path) {
    MessageHandler::info("Logging out...");
    MessageHandler::info("Returning to login...");
    start(credentials_path);
}
