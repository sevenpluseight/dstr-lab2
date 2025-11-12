#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "emergency_manager.hpp"
#include "path_utils.hpp"

namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[31m"; // Pending
    const std::string YELLOW  = "\033[33m"; // Processing
    const std::string GREEN   = "\033[32m"; // Completed
}

EmergencyManager::~EmergencyManager() {
    // Clear the emergency case list
    Node* current = head;
    while (current) {
        Node* tmp = current;
        current = current->next;
        delete tmp;
    }

    // Clear the patient data list
    PatientNode* pCurrent = patientHead;
    while (pCurrent) {
        PatientNode* pTmp = pCurrent;
        pCurrent = pCurrent->next;
        delete pTmp;
    }

    // Clear the emergency type list
    TypeNode* tCurrent = typeHead;
    while (tCurrent) {
        TypeNode* tTmp = tCurrent;
        tCurrent = tCurrent->next;
        delete tTmp;
    }

    // Clear the supply type list
    SupplyTypeNode* stCurrent = supplyTypeHead;
    while (stCurrent) {
        SupplyTypeNode* stTmp = stCurrent;
        stCurrent = stCurrent->next;
        delete stTmp;
    }

    // Clear the unique supply name list
    UniqueSupplyNode* usCurrent = uniqueSupplyHead;
    while (usCurrent) {
        UniqueSupplyNode* usTmp = usCurrent;
        usCurrent = usCurrent->next;
        delete usTmp;
    }

    // Clear the supply data list
    SupplyNode* sCurrent = supplyHead;
    while (sCurrent) {
        SupplyNode* sTmp = sCurrent;
        sCurrent = sCurrent->next;
        delete sTmp;
    }

    clearAmbulanceList();
}

// Adds a new patient to the front of the patient list
void EmergencyManager::addPatient(const std::string& id, const std::string& name) {
    PatientNode* newNode = new PatientNode;
    newNode->patientID = id;
    newNode->patientName = name;
    newNode->next = patientHead; // Add to front
    patientHead = newNode;
}

// Loads patient data into the list *once*
void EmergencyManager::loadPatientData(const std::string& patientDataFile) {
    std::ifstream file(patientDataFile);
    if (!file.is_open()) {
        MessageHandler::warning("Patient data CSV not found: " + patientDataFile);
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string patientID, patientName;
        
        std::getline(ss, patientID, ','); 
        std::getline(ss, patientName, ','); 
        
        if (!patientID.empty() && !patientName.empty()) {
            trim(patientID); 
            trim(patientName);
            addPatient(patientID, patientName);
        }
    }
    file.close();
}

// Gets a patient name from the in-memory list
std::string EmergencyManager::getPatientName(const std::string& patientID) const {
    PatientNode* current = patientHead;
    while (current) {
        if (current->patientID == patientID) {
            return current->patientName; // Found it
        }
        current = current->next;
    }
    return "Unknown"; // Not found
}

// Checks if a type already exists in the list
bool EmergencyManager::typeExists(const std::string& type) const {
    std::string upperType = toUpper(type); 
    TypeNode* current = typeHead;
    while (current) {
        if (current->typeName == upperType) { 
            return true;
        }
        current = current->next;
    }
    return false;
}

// Adds a new type to the list in alphabetical order 
void EmergencyManager::addType(const std::string& type) {
    std::string upperType = toUpper(type); // Convert to upper

    if (upperType.empty() || typeExists(upperType)) {
        return;
    }

    TypeNode* newNode = new TypeNode;
    newNode->typeName = upperType;
    newNode->next = nullptr;

    // Case 1: The list is empty or the new type comes before the head
    if (!typeHead || upperType < typeHead->typeName) {
        newNode->next = typeHead;
        typeHead = newNode;
        return;
    }

    // Case 2: Insert in the middle or at the end
    TypeNode* current = typeHead;
    while (current->next && current->next->typeName < upperType) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}

// Prints a numbered list of all unique types
int EmergencyManager::printAllTypes() const {
    TypeNode* current = typeHead;
    int i = 0;
    while (current) {
        i++;
        std::cout << i << ". " << current->typeName << "\n";
        current = current->next;
    }
    return i; // Return the count of types
}

// Gets a type name string by its 1-based index
std::string EmergencyManager::getTypeByIndex(int index) const {
    TypeNode* current = typeHead;
    int i = 0;
    while (current) {
        i++;
        if (i == index) {
            return current->typeName;
        }
        current = current->next;
    }
    return "Unknown";
}

// Load from CSV
void EmergencyManager::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        MessageHandler::warning("Emergency CSV not found, starting empty.");
        return;
    }

    std::string line;
    std::getline(file, line);

    bool hasNameColumn = (line.find("Patient_Name") != std::string::npos);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        EmergencyCase ec;
        
        std::getline(ss, ec.case_id, ',');
        std::getline(ss, ec.patient_id, ',');

        if (hasNameColumn) {
            std::getline(ss, ec.patient_name, ',');
        } else {
            ec.patient_name = getPatientName(ec.patient_id);
        }

        std::getline(ss, ec.emergency_type, ','); 

        std::string prio;
        std::getline(ss, prio, ',');
        
        if (prio.empty()) prio = "5"; 
        try {
            ec.priority_level = std::stoi(prio);
        } catch (const std::invalid_argument&) {
            ec.priority_level = 5; 
        }

        std::getline(ss, ec.status, ','); 

        std::getline(ss, ec.timestamp_logged, ','); 

        std::getline(ss, ec.timestamp_processed, ','); 

        std::getline(ss, ec.ambulance_id, ','); 

        trim(ec.status); 
        trim(ec.ambulance_id);
        addCase(ec); // insert in priority order
    }
    file.close();
}

// Save to CSV
void EmergencyManager::saveToCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "Case_ID,Patient_ID,Emergency_Type,Priority_Level,Status,Timestamp_Logged,Timestamp_Processed,Ambulance_ID\n";

    Node* current = head;
    while (current) {
        EmergencyCase& ec = current->data;
        file << ec.case_id << "," << ec.patient_id << ","
             << ec.emergency_type << "," << ec.priority_level << ","
             << ec.status << "," << ec.timestamp_logged << "," 
             << ec.timestamp_processed << "," << ec.ambulance_id << "\n";
        current = current->next;
    }
    file.close();
}

// Helper function to print a single case row
void printCaseRow(const EmergencyCase& ec) {
    std::string statusColor;

    if (ec.status == "Pending") {
        statusColor = Color::RED;
    } else if (ec.status == "Processing") {
        statusColor = Color::YELLOW;
    } else if (ec.status == "Completed") {
        statusColor = Color::GREEN;
    } else {
        statusColor = Color::RESET;
    }

    std::cout << std::left
              << std::setw(13)  << ec.case_id
              << std::setw(13) << ec.patient_id
              << std::setw(19) << ec.patient_name
              << std::setw(29) << ec.emergency_type
              << std::setw(10) << std::to_string(ec.priority_level)
              << statusColor << std::setw(12) << ec.status << Color::RESET
              << std::setw(22) << ec.timestamp_logged;

    if (!ec.timestamp_processed.empty()) {
        std::cout << std::setw(22) << ec.timestamp_processed
                  << std::setw(10) << ec.ambulance_id;
    }

    std::cout << "\n";
}

void EmergencyManager::printAllCases() const {
    if (!head) {
        MessageHandler::info("No emergency cases available.");
        return;
    }

    std::cout << "\n--- All Emergency Cases ---\n";

    std::cout << std::left
              << std::setw(13)  << "Case_ID"
              << std::setw(13) << "Patient_ID"
              << std::setw(19) << "Patient_Name"
              << std::setw(29) << "Emergency_Type"
              << std::setw(10) << "Priority"
              << std::setw(12) << "Status"
              << std::setw(22) << "Logged"
              << std::setw(22) << "Processed"
              << std::setw(10) << "Ambulance"
              << "\n";
    std::cout << std::string(151, '-') << "\n";

    Node* current = nullptr;
    bool casesFound = false;

 // Loop for each priority level from 1 to 5
    for (int priority = 1; priority <= 5; ++priority) {

        // "Pending" 
        current = head; // Reset pointer to the start of the list
        while (current) {
            if (current->data.priority_level == priority && current->data.status == "Pending") {
                printCaseRow(current->data);
                casesFound = true;
            }
            current = current->next;
        }

        // "Processing"
        current = head; // Reset pointer to the start of the list
        while (current) {
            if (current->data.priority_level == priority && current->data.status == "Processing") {
                printCaseRow(current->data);
                casesFound = true;
            }
            current = current->next;
        }

        // "Completed" 
        current = head; // Reset pointer to the start of the list
        while (current) {
            if (current->data.priority_level == priority && current->data.status == "Completed") {
                printCaseRow(current->data);
                casesFound = true;
            }
            current = current->next;
        }
    }
    
    if (!casesFound) {
         std::cout << "(No emergency cases found)\n";
    }
}

// Print cases by status
void EmergencyManager::printCasesByStatus(const std::string& status) const {
    if (!head) {
        MessageHandler::info("No emergency cases available.");
        return;
    }

    std::cout << "\n--- " << status << " Cases ---\n";

    std::cout << std::left
              << std::setw(13)  << "Case_ID"
              << std::setw(13) << "Patient_ID"
              << std::setw(19) << "Patient_Name"
              << std::setw(29) << "Emergency_Type"
              << std::setw(10) << "Priority"
              << std::setw(12) << "Status"
              << std::setw(22) << "Logged"
              << std::setw(22) << "Processed"
              << std::setw(10) << "Ambulance"
              << "\n";
    std::cout << std::string(151, '-') << "\n"; 

    Node* current = head;
    bool found = false;
    while (current) {
        if (current->data.status == status) {
            printCaseRow(current->data);
            found = true;
        }
        current = current->next;
    }

    if (!found)
        std::cout << "(No " << status << " cases found)\n";
}

// Add case (insert by priority)
void EmergencyManager::addCase(const EmergencyCase& ec) {
    Node* newNode = new Node{ec, nullptr};

    addType(ec.emergency_type);

    // Empty list or higher priority than head
    if (!head || ec.priority_level < head->data.priority_level) {
        newNode->next = head;
        head = newNode;
        return;
    }

    Node* current = head;
    while (current->next && current->next->data.priority_level <= ec.priority_level) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}

// Peeks at the highest priority "Pending" case
EmergencyCase EmergencyManager::getHighestPriorityPendingCase() const {
    Node* current = head;
    while (current) {
        if (current->data.status == "Pending") {
            return current->data;
        }
        current = current->next;
    }
    // If no "Pending" cases are found, return an empty one
    return EmergencyCase{}; 
}

// Finds and removes the highest-priority "Pending" case
EmergencyCase EmergencyManager::popHighestPriorityPendingCase() {
    if (!head) {
        return EmergencyCase{}; // List is empty
    }

    Node* current = head;
    Node* prev = nullptr;

    // Loop until it finds the first "Pending" case
    while (current && current->data.status != "Pending") {
        prev = current;
        current = current->next;
    }

    // Case 1: No "Pending" cases were found
    if (!current) {
        return EmergencyCase{}; 
    }

    // Case 2: The "Pending" case is the head of the list
    if (prev == nullptr) {
        head = current->next; // Re-point head
    } 
    // Case 3: The "Pending" case is in the middle or at the end
    else {
        prev->next = current->next; // Unlink the node
    }

    // Save the data, delete the node and return the data
    EmergencyCase ec = current->data;
    delete current;
    return ec;
}

// Update case
void EmergencyManager::updateCase(const EmergencyCase& ec) {
    Node* current = head;
    while (current) {
        if (current->data.case_id == ec.case_id) {
            current->data = ec;
            return;
        }
        current = current->next;
    }
    addCase(ec);
}

// Generate next Case ID
std::string EmergencyManager::generateNextCaseID() {
    int maxID = 3350; // Start from a base, CASE-3101
    Node* current = head;

    while (current) {
        const std::string& id = current->data.case_id;
        if (id.rfind("CASE-", 0) == 0) {
            try {
                int num = std::stoi(id.substr(5)); // Get number after "CASE-"
                if (num > maxID) maxID = num;
            } catch (...) {}
        }

        current = current->next;
    }

    std::ostringstream oss;
    oss << "CASE-" << (maxID + 1);
    return oss.str();
}

// Adds a unique supply type to the list
void EmergencyManager::addSupplyType(const std::string& type) {
    if (type.empty()) return;
    SupplyTypeNode* current = supplyTypeHead;
    while (current) {
        if (current->typeName == type) return; // Already exists
        current = current->next;
    }
    SupplyTypeNode* newNode = new SupplyTypeNode;
    newNode->typeName = type;
    newNode->next = supplyTypeHead;
    supplyTypeHead = newNode;
}

// Adds a unique supply NAME to the list
void EmergencyManager::addUniqueSupply(const std::string& name, const std::string& type) {
    if (name.empty() || type.empty()) return;
    UniqueSupplyNode* current = uniqueSupplyHead;
    while (current) {
        if (current->supplyName == name && current->supplyType == type) {
            return; // Already exists
        }
        current = current->next;
    }
    UniqueSupplyNode* newNode = new UniqueSupplyNode;
    newNode->supplyName = name;
    newNode->supplyType = type;
    newNode->next = uniqueSupplyHead;
    uniqueSupplyHead = newNode;
}

// Adds a supply item to the main supply list
void EmergencyManager::addSupply(const std::string& id, const std::string& name, const std::string& type, int stockQty) {
    SupplyNode* newNode = new SupplyNode;
    newNode->supplyID = id;
    newNode->supplyName = name;
    newNode->supplyType = type;
    newNode->stockQuantity = stockQty;
    newNode->next = supplyHead; // Add to front
    supplyHead = newNode;
}

// Loads all supply data from medical_supply.csv
void EmergencyManager::loadSupplyData(const std::string& supplyDataFile) {
    std::ifstream file(supplyDataFile);
    if (!file.is_open()) {
        MessageHandler::warning("Medical supply CSV not found: " + supplyDataFile);
        return;
    }
    std::string line;
    std::getline(file, line); // Skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string supplyID, supplyName, supplyType, stockQtyStr;
        std::getline(ss, supplyID, ',');   
        std::getline(ss, supplyName, ','); 
        std::getline(ss, supplyType, ','); 
        std::getline(ss, stockQtyStr, ',');
        if (!supplyID.empty() && !supplyName.empty() && !supplyType.empty()) {
            trim(supplyID); 
            trim(supplyName);
            trim(supplyType);
            trim(stockQtyStr);

            int stockQty = 0;
            try {
                if(!stockQtyStr.empty()) {
                    stockQty = std::stoi(stockQtyStr);
                }
            } catch (const std::invalid_argument&) {
                MessageHandler::warning("Invalid stock quantity for supply ID: " + supplyID + ". Defaulting to 0.");
            }
            addSupply(supplyID, supplyName, supplyType, stockQty);
            addSupplyType(supplyType);
            addUniqueSupply(supplyName, supplyType); 
        }
    }
    file.close();
}

// Prints a numbered menu of unique supply types
int EmergencyManager::printSupplyTypes() const {
    SupplyTypeNode* current = supplyTypeHead;
    int i = 0;
    while (current) {
        i++;
        std::string fullName; // Variable to hold the pretty name

        if (current->typeName == "MED") {
            fullName = "Medication (MED)";
        } else if (current->typeName == "EQP") {
            fullName = "Equipment (EQP)";
        } else if (current->typeName == "PPE") {
            fullName = "Personal Protective Equipment (PPE)";
        } else {
            fullName = current->typeName; // Fallback for any other types
        }
        std::cout << i << ". " << fullName << "\n";
        current = current->next;
    }
    return i; 
}

// Gets the string name of a supply type by its menu index
std::string EmergencyManager::getSupplyTypeByIndex(int index) const {
    SupplyTypeNode* current = supplyTypeHead;
    int i = 0;
    while (current) {
        i++;
        if (i == index) return current->typeName;
        current = current->next;
    }
    return "Unknown";
}

// Prints a menu of unique supply NAMES for a given type
int EmergencyManager::printUniqueSuppliesByType(const std::string& type) const {
    UniqueSupplyNode* current = uniqueSupplyHead;
    int i = 0;
    while (current) {
        if (current->supplyType == type) {
            i++;
            std::cout << i << ". " << current->supplyName << "\n";
        }
        current = current->next;
    }
    return i; // Return the count
}

// Gets the string NAME of a unique supply
std::string EmergencyManager::getUniqueSupplyNameByTypeAndIndex(const std::string& type, int index) const {
    UniqueSupplyNode* current = uniqueSupplyHead;
    int i = 0;
    while (current) {
        if (current->supplyType == type) {
            i++;
            if (i == index) return current->supplyName;
        }
        current = current->next;
    }
    return "Unknown";
}

// Prints a menu of all BATCHES for a given supply NAME
int EmergencyManager::printBatchesForSupply(const std::string& supplyName) const {
    SupplyNode* current = supplyHead;
    int i = 0;
    while (current) {
        if (current->supplyName == supplyName) {
            i++;
            std::cout << i << ". " << current->supplyName 
                      << " (ID: " << current->supplyID << ")\n";
        }
        current = current->next;
    }
    return i; // Return the count
}

// Gets a pointer to a specific BATCH
EmergencyManager::SupplyNode* EmergencyManager::getBatchBySupplyNameAndIndex(const std::string& supplyName, int index) const {
    SupplyNode* current = supplyHead;
    int i = 0;
    while (current) {
        if (current->supplyName == supplyName) {
            i++;
            if (i == index) return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Finds a case by its ID and returns a pointer
EmergencyCase* EmergencyManager::getCaseByID(const std::string& caseID) {
    Node* current = head;
    while (current) {
        if (current->data.case_id == caseID) {
            return &(current->data); // Return pointer to the data
        }
        current = current->next;
    }
    return nullptr; // Not found
}

// Appends a supply usage record to the log
void EmergencyManager::logSupplyUsage(const EmergencyCase& ec, const std::string& supplyID, 
                                        const std::string& supplyName, int quantity) 
{
    std::string logFilePath = getDataFilePath("supply_usage_log.csv");
    bool fileExists = false;
    { 
        std::ifstream checker(logFilePath);
        if (checker.good()) fileExists = true;
    }
    std::ofstream logFile(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        MessageHandler::error("FATAL: Could not open supply usage log file at: " + logFilePath);
        return;
    }
    if (!fileExists) {
        logFile << "Case_ID,Patient_ID,Supply_Batch_ID,Supply_Name,Quantity_Used,Status\n";
    }
    logFile << ec.case_id << ","
            << ec.patient_id << ","
            << supplyID << ","
            << supplyName << ","
            << quantity << ","
            << "Not Deducted" << "\n";
    logFile.close();
}

void EmergencyManager::clearAmbulanceList() {
    AmbulanceNode* current = ambHead;
    while (current) {
        AmbulanceNode* tmp = current;
        current = current->next;
        delete tmp;
    }
    ambHead = nullptr;
}

void EmergencyManager::loadAmbulanceData(const std::string& filename) {
    clearAmbulanceList(); // Clear old data first
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        MessageHandler::warning("Ambulance schedule CSV not found: " + filename);
        return;
    }
    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        Ambulance amb;
        
        std::getline(ss, amb.ambulance_id, ','); 
        std::getline(ss, amb.driver_name, ',');   
        std::getline(ss, amb.driver_status, ','); 
        std::getline(ss, token, ','); 
        std::getline(ss, token, ',');
        std::getline(ss, token, ','); 
        std::getline(ss, token, ','); 
        std::getline(ss, amb.assigned_case_id, ','); 
        std::getline(ss, amb.ambulance_status, ','); 

        trim(amb.ambulance_id);
        trim(amb.driver_name);
        trim(amb.driver_status);
        trim(amb.assigned_case_id);
        trim(amb.ambulance_status);

        // Add to our list (to the front)
        AmbulanceNode* newNode = new AmbulanceNode{amb, nullptr};
        newNode->next = ambHead;
        ambHead = newNode;
    }
    file.close();
}

std::string EmergencyManager::getFirstAvailableAmbulanceID() const {
    AmbulanceNode* current = ambHead;
    while (current) {
        // Find ambulances that are ready for a new case
        bool isReady = (current->data.ambulance_status.find("On Duty") != std::string::npos) ||
                       (current->data.ambulance_status.find("Available") != std::string::npos);

        if (isReady) {
            // Check if it is free from 'Processing' list
            if (!isAmbulanceOnProcessingCase(current->data.ambulance_id)) 
            {
                return current->data.ambulance_id; // Found an ambulance that is truly available
            }
        }
        current = current->next;
    }
    return ""; 
}

bool EmergencyManager::isAmbulanceOnProcessingCase(const std::string& ambID) const {
    Node* current = head; // Start at the beginning of the emergency case list

    while (current) {
        // Check if a case is "Processing" AND has the ambulance ID we're looking for
        if (current->data.status == "Processing" && current->data.ambulance_id == ambID) {
            return true; // Found it, the ambulance is busy
        }
        current = current->next;
    }

    return false; // Not found, the ambulance is free
}