#include "entities.hpp"
#include <iostream>

Patient::Patient() {
    patient_id = "";
    name = "";
    age = "";
    gender = "";
    bloodType = "";
    height = "";
    weight = "";
    BMI = "";
    temperature = "";
    heartRate = "";
    bloodPressure = "";
    condition = "";
}

Patient::Patient(DynamicArray patientElements) {
    patient_id = patientElements.getElementAt(0);
    name = patientElements.getElementAt(1);
    age = patientElements.getElementAt(2);
    gender = patientElements.getElementAt(3);
    bloodType = patientElements.getElementAt(4);
    height = patientElements.getElementAt(5);
    weight = patientElements.getElementAt(6);
    BMI = patientElements.getElementAt(7);
    temperature = patientElements.getElementAt(8);
    heartRate = patientElements.getElementAt(9);
    bloodPressure = patientElements.getElementAt(10);
    condition = patientElements.getElementAt(11);
}

std::string Patient::getLine() {
    std::string line = patient_id + "," + name + "," + age + "," + gender + "," + bloodType + "," + height + "," + weight + "," +
                       BMI + "," + temperature + "," + heartRate + "," + bloodPressure + "," + condition;
    return line;
}