#ifndef AMBULANCE_DISPATCHER_HPP
#define AMBULANCE_DISPATCHER_HPP

#include <vector>
#include <string>
#include "../../utils/entities.hpp"

class AmbulanceDispatcher {
private:
    std::vector<Ambulance> ambulances;

    void displaySchedule() const;
    void removeAmbulance();  // new

public:
    void displayMenu();
    void registerAmbulance();
    void rotateAmbulanceShift();
    void dispatchAmbulance(); // stub for now
    void loadSampleData();
};

#endif // AMBULANCE_DISPATCHER_HPP
