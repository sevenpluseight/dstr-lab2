#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include "login.hpp"

/**
 * @class SessionManager
 * @brief Handles user session flow including logout and re-login loop
 *
 * @usage
 * For all role classes
 * 1. Include this header: #include "session_manager.hpp"
 * 2. When the user selects "Logout" in their menu:
 *      @code
 *      SessionManager::logout(credentials_file);
 *      @encode
 *
 */
class SessionManager {
public:
    /**
     * @brief Starts the login loop
     * @details After successful login and role menu execution, the system waits for logout then restarts login
     *
     * @param credentials_path Path to the user credentials CSV
     */
    static void start(const std::string& credentials_path);

    /**
     * @brief Handles logout for any role
     * @details Displays a message and triggers re-login by calling start();
     */
    static void logout(const std::string& credentials_path);
};

#endif
