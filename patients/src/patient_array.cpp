#include "../include/patient_array.hpp"

PatientArray::PatientArray() {
    size = 0;
    capacity = 1;
    array = new Patient[capacity];
}

PatientArray::PatientArray(int inputCapacity) {
    size = 0;
    capacity = inputCapacity;
    array = new Patient[capacity];
}

int PatientArray::getSize() {
    return size;
}

int PatientArray::getCapacity() {
    return capacity;
}

void PatientArray::growArray() {
    capacity = capacity * 2;
    Patient* newArray = new Patient[capacity];
    for (int i = 0; i < size; i++) {
        newArray[i] = array[i];
    }
    delete[] array;
    array = newArray;
}

void PatientArray::shrinkArray() {
    capacity = size;
    Patient* newArray = new Patient[capacity];
    for (int i = 0; i < capacity; i++) {
        newArray[i] = array [i];
    }
    delete[] array;
    array = newArray;
}

void PatientArray::appendArray(Patient patient) {
    if (capacity == size) {
        growArray();
    }
    array[size] = patient;
    size++;
}

void PatientArray::popArray() {
    array[size - 1] = Patient();
    size--;
    if (capacity == size) {
        shrinkArray();
    }
}

void PatientArray::clearArray() {
    size = 0;
    capacity = 1;
    Patient* newArray = new Patient[capacity];
    delete[] array;
    array = newArray;
}

bool PatientArray::isInArray(std::string patientID) {
    for(int i = 0; i < size; i++) {
        if (array[i].patient_id == patientID) {
            return true;
        }
    }
    return false;
}

Patient PatientArray::getPatientWithID(std::string patientID) {
    for(int i = 0; i < size; i++) {
        if (array[i].patient_id == patientID) {
            return array[i];
        }
    }
    return Patient();
}