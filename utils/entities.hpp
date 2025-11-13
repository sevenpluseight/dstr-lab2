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
    int quantity; /// Quantity received
    // int max_capacity; /// Maximum allowed stock capacity
    std::string status; /// Status (Available, Expired)
    std::string supplier_name; /// Supplier name
    std::string timestamp_added; /// Date/time when supply was added
    std::string expiry_date; /// Expiration date
};

struct SupplyUsageLog {
    std::string case_id;
    std::string patient_id;
    std::string supply_batch_id;
    std::string supply_name;
    int quantity_used;
    std::string status;
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

struct ShiftRecord {
    std::string shift_id;
    std::string ambulance_id;
    std::string driver_name;
    std::string shift_start;
    std::string shift_end;
    int shift_duration_hours;
    std::string assigned_case_id;
    int cases_handled;
    double total_distance_km;
    int oxygen_used;
    int medicine_used;
    std::string status_at_end;
};

struct Ambulance {
    std::string ambulance_id;
    std::string driver_name;
    std::string driver_status;
    std::string shift_start;
    std::string shift_end;
    int shift_duration;
    std::string next_rotation_time;
    std::string assigned_case_id;
    std::string ambulance_status;
    int oxygen_tank;
    int first_aid_kit;
    int medicine_units;
    std::string last_service_date;
    std::string location;
    DynamicArray shift_history;
};





#endif
