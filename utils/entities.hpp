#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include <string>
// using namespace std;

/**
 * @brief Defines shared data structures (entities) used across all hospital modules
 *
 * @note
 * - Do not implement any logic or functions here - only struct declarations
 * - Include this header in all role modules that need shared entity access
 * - Update this file CAREFULLY if attributes are modified as it impacts all modules
 * - If grows too big, can refactor into sub-headers
 *
 * @usage
 * - Use in cross-module operations (e.g., when Emergency Department needs to read `Patients` data)
 * - Use to maintain consistent data definitions across all system components
 */

// Patient Admission Clerk



// Medical Supply Manager
/**
 * @struct Supply
 * @brief Represents a single medical supply item in the hospital's inventory
 *
 * @details
 * Used by the Medical Supply Manager to record, manage and track supply stock
 */
struct Supply {
    std::string supply_batch_id; /// Unique ID
    std::string name; /// Name of the supply
    std::string supply_type; /// Type of supply (Medical, Equipment, Protective Personal Equipment [PPE])
    int quantity; /// Current available quantity
    int min_required; /// Minimum required quantity before low stock
    int max_capacity; /// Maximum allowed stock capacity
    std::string status; /// Status (Available, Low Stock, Expired)
    std::string supplier_name; /// Supplier name
    std::string timestamp_added; /// Date/time when supply was added
    std::string expiry_date; /// Expiration date
};

// Emergency Department Officer



// Ambulance Dispatcher





#endif
