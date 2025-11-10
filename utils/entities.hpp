#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include <string>
#include "dynamic_array.hpp"
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
 * - Can also be included and accessed by its own module (self-access)
 * - Use to maintain consistent data definitions across all system components
 *
 * ℹ️ If you find issues with these structs, please raise it in the WhatsApp group before making changes
 */

// Patient Admission Clerk

struct Patient {
        std::string patient_id;
        std::string name;
        std::string age;
        std::string gender;
        std::string bloodType;
        std::string height;
        std::string weight;
        std::string BMI;
        std::string temperature;
        std::string heartRate;
        std::string bloodPressure;
        std::string condition;
        Patient();
        Patient(DynamicArray patientElement);
        std::string getLine();
};

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
/**
 * @struct EmergencyCase
 * @brief Represents a single emergency admission record
 */
struct EmergencyCase {
    std::string case_id;           
    std::string patient_id;        
    std::string patient_name;      
    std::string emergency_type;    
    int priority_level;            
    std::string status;            
    std::string timestamp_logged;  
    std::string timestamp_processed; 
    std::string ambulance_id;      
};

struct Ambulance {
    std::string ambulance_id;       /// Unique ID for each ambulance (e.g., "AMB001")
    std::string driver_name;        /// Name of the assigned driver
    std::string driver_status;      /// Current status of the driver (Available, OnDuty, Unavailable)
    std::string shift_start;        /// Shift start time (YYYY-MM-DD HH:MM)
    std::string shift_end;          /// Shift end time (YYYY-MM-DD HH:MM)
    int shift_duration;             /// Duration of shift in hours
    std::string assigned_case_id;   /// Linked case ID from Emergency Department (if any)
    std::string ambulance_status;   /// Status of ambulance (Available, On Duty, Under Maintenance)
    std::string location;           /// Current location or dispatch base
};





#endif
