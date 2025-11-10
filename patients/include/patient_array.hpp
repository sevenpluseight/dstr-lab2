#ifndef PATIENTARRAY_H
#define PATIENTARRAY_H

#include <iostream>
#include <string>
#include "entities.hpp"

class PatientArray {
    private:
        int size;
        int capacity;
        Patient* array;
    public:
        PatientArray();
        PatientArray(int inputCapacity);
        int getSize();
        int getCapacity();
        void growArray();
        void shrinkArray();
        void appendArray(Patient patient);
        void popArray();
        void clearArray();
        bool isInArray(std::string patientID);
        Patient getPatientWithID(std::string patientID);
};

#endif