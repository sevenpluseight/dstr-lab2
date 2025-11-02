#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <string>
#include <fstream>
#include <iostream>

#include "message_handler.hpp"

#define MAX_USERS 100 // Maximum number of users supported

/**
 * @struct UserCredential
 * @brief Represents a single user credential
 *
 */
struct UserCredential {
    std::string username;
    std::string password;
    std::string role;
};

/**
 * @class Login
 * @brief Handles role-based login for all roles
 */
class Login {
private:
    UserCredential users[MAX_USERS];
    int user_count;
    std::string credentials_file;

    /**
     * @brief Load user credentials from CSV into array
     */
    void loadCredentials();

    /**
     * @brief Attempt login for a user
     * @param username Input username
     * @param password Input password
     * @return Role of the user if successful, empty string if failed
     */
    std::string attemptLogin(const std::string& username, const std::string& password);

public:
    /**
     * @brief Constructs a Login instance and loads user credentials
     * @param filepath Path to CSV file with credentials
     */
    Login(const std::string& filepath);

    /**
     * @brief Prompts the user for login
     */
    bool promptLogin();
};

#endif