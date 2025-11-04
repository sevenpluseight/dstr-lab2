#include "session_manager.hpp"
#include "path_utils.hpp"

/**
 * @brief Starts the login loop
 * @param credentials_path Path to user credentials file
 */
void SessionManager::start(const std::string& credentials_path) {
    MessageHandler::info("\nHospital Management System");
    std::string full_path = getDataFilePath(credentials_path);
    Login login(full_path);
    while (login.promptLogin()) {
        // Loop continues as long as promptLogin returns true (user wants to re-login)
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
