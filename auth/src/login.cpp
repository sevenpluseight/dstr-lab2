#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>

#include "login.hpp"
#include "message_handler.hpp"
#include "string_utils.hpp"

/**
 * @brief Constructor for Login
 * @param filepath Path to the user credentials CSV file
 */
Login::Login(const std::string& filepath) {
    credentials_file = filepath;
    user_count = 0;
    loadCredentials();
}

/**
 * @brief Load user credentials from CSV into static array
 * @details Supports a maximum of MAX_USERS
 */
void Login::loadCredentials() {
    std::ifstream file(credentials_file.c_str());
    if (!file.is_open()) {
        MessageHandler::error("Unable to open credentials file: " + credentials_file);
        return;
    }

    std::string line;
    std::getline(file, line); // Skip CSV header

    while (std::getline(file, line) && user_count < MAX_USERS) {
        std::stringstream ss(line);
        std::string username, password, role;

        if (!std::getline(ss, username, ',')) continue;
        if (!std::getline(ss, password, ',')) continue;
        if (!std::getline(ss, role, ',')) continue;

        username = trim(username);
        password = trim(password);
        role     = trim(role);

        users[user_count].username = username;
        users[user_count].password = password;
        users[user_count].role = role;

        user_count++;
    }
    file.close();
}

/**
 * @brief Attempt login with given username and password
 * @param username Input username
 * @param password Input password
 * @return Role string if login successful, empty string if failed
 */
std::string Login::attemptLogin(const std::string &username, const std::string &password) {
    for (int i = 0; i < user_count; i++) {
        if (users[i].username == username && users[i].password == password) {
            return users[i].role;
        }
    }
    return ""; // Login failed
}

/**
 * @brief Prompt the user for login and redirect to role menu
 * @details
 * - Re-prompts if login fails (invalid credentials)
 * - If username is "exit" (case-sensitive), the program terminates immediately
 */
void Login::promptLogin() {
    // Synchronize cout and cerr to maintain correct order
    std::ios::sync_with_stdio(true);
    std::cin.tie(nullptr);

    std::string username, password;

    while (true) {
        std::cout << "Please enter your username (or type 'exit' to quit): ";
        std::cout.flush();
        std::getline(std::cin, username);
        trim(username);

        // Convert to lowercase for "exit"
        std::string lower_username = username;
        for (auto &c : lower_username)
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

        // Exit option
        if (lower_username == "exit") {
            MessageHandler::info("Program terminated.");
            std::cout.flush();
            exit(0);
        }

        std::cout << "Please enter your password: ";
        std::cout.flush();
        std::getline(std::cin, password);
        trim(password);

        std::string role = attemptLogin(username, password);

        // Handle invalid login
        if (role.empty()) {
            std::cout << std::endl;
            MessageHandler::error("Login failed. Incorrect username or password.");
            std::cout << std::endl;
            continue; // Retry login
        }

        MessageHandler::info("Login successful. Your role is " + role + "\n");

        enum RoleType { PATIENT_ADMISSION_CLERK, MEDICAL_SUPPLY_MANAGER, EMERGENCY_DEPARTMENT_OFFICER, AMBULANCE_DISPATCHER, UNKNOWN };
        RoleType user_role = UNKNOWN;

        if (role == "PatientAdmissionClerk") user_role = PATIENT_ADMISSION_CLERK;
        else if (role == "MedicalSupplyManager") user_role = MEDICAL_SUPPLY_MANAGER;
        else if (role == "EmergencyDepartmentOfficer") user_role = EMERGENCY_DEPARTMENT_OFFICER;
        else if (role == "AmbulanceDispatcher") user_role = AMBULANCE_DISPATCHER;

        switch (user_role) {
            case PATIENT_ADMISSION_CLERK:
                MessageHandler::warning("Patient Admission Clerk menu not implemented yet");
                // Once implemented, comment out the above MessageHandler line
                // and uncomment the block below:
                // {
                //     PatientAdmissionClerk pac;
                //     pac.displayMenu();
                // }
                break;

            case MEDICAL_SUPPLY_MANAGER:
                MessageHandler::warning("Medical Supply Manager menu not implemented yet");
                // Once implemented, comment out the above MessageHandler line
                // and uncomment the block below:
                // {
                //     MedicalSupplyManager msm;
                //     msm.displayMenu();
                // }
                break;

            case EMERGENCY_DEPARTMENT_OFFICER:
                MessageHandler::warning("Emergency Department Officer menu not implemented yet");
                // Once implemented, comment out the above MessageHandler line
                // and uncomment the block below:
                // {
                //     EmergencyDepartmentOfficer eod;
                //     eod.displayMenu();
                // }
                break;

            case AMBULANCE_DISPATCHER:
                MessageHandler::warning("Ambulance Dispatcher menu not implemented yet");
                // Once implemented, comment out the above MessageHandler line
                // and uncomment the block below:
                // {
                //     AmbulanceDispatcher ab;
                //     ab.displayMenu();
                // }
                break;

            default:
                MessageHandler::error("Unknown role: " + role);
                break;
        }

        break; // Exit loop on successful login
    }
}
