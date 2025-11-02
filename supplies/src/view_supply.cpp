#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "medical_supply_manager.hpp"
#include "message_handler.hpp"
#include "stack.hpp"

/**
 * @brief Displays all current supplies in the stack
 */
void MedicalSupplyManager::viewCurrentSupply() {
    if (stack.isStackEmpty()) {
        MessageHandler::warning("No supplies available.");
        return;
    }

    std::cout << "\nCURRENT SUPPLY INVENTORY\n";

    // Define column widths
    const int wBatchID = 20;
    const int wName    = 25;
    const int wType    = 8;
    const int wQty     = 8;
    const int wMin     = 8;
    const int wMax     = 8;
    const int wStatus  = 12;
    const int wSupplier= 28;
    const int wRecv    = 18;
    const int wExp     = 18;
    std::cout << std::string(148, '-') << "\n";

    // Print table header
    std::cout << std::left
              << std::setw(wBatchID) << "Supply Batch ID"
              << std::setw(wName)    << "Name"
              << std::setw(wType)    << "Type"
              << std::setw(wQty)     << "Qty"
              << std::setw(wMin)     << "MinReq"
              << std::setw(wMax)     << "MaxCap"
              << std::setw(wStatus)  << "Status"
              << std::setw(wSupplier)<< "Supplier"
              << std::setw(wRecv)    << "Received"
              << std::setw(wExp)     << "Expiry"
              << "\n";

    // Print line separator
    std::cout << std::string(
        wBatchID + wName + wType + wQty + wMin + wMax + wStatus + wSupplier + wRecv + wExp,
        '-') << "\n";

    // Print each supply record from top (newest) to bottom (oldest)
    for (int i = stack.getTopIndex(); i >= 0; --i) {
        Supply s = stack.getSupplyAt(i);

        std::cout << std::left
                  << std::setw(wBatchID) << s.supply_batch_id
                  << std::setw(wName)    << s.name
                  << std::setw(wType)    << s.supply_type
                  << std::setw(wQty)     << s.quantity
                  << std::setw(wMin)     << s.min_required
                  << std::setw(wMax)     << s.max_capacity
                  << std::setw(wStatus)  << s.status
                  << std::setw(wSupplier)<< s.supplier_name
                  << std::setw(wRecv)    << s.timestamp_added
                  << std::setw(wExp)     << s.expiry_date
                  << "\n";
    }
}
