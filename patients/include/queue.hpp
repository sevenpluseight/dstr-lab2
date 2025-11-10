#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include "entities.hpp"
#include "string_utils.hpp"

enum PatientStatus {ADMITTED ,DISCHARGED, NONE};

inline std::string patientStatusToString(const PatientStatus status) {
    switch (status)
    {
    case ADMITTED:
        return "ADMITTED";
    case DISCHARGED:
        return "DISCHARGED";
    default:
        return "NONE";
    }
};

inline PatientStatus parsePatientStatus(const std::string& status) {
    if(trim(status) == "ADMITTED") {
        return ADMITTED;
    }
    if(trim(status) == "DISCHARGED") {
        return DISCHARGED;
    }
    return NONE;
};

class Node {
    public:
        std::string patient_id;
        std::string condition;
        std::string admission;
        PatientStatus status;
        std::string discharge;
        Node* next;
};

class Queue {
    private:
        Node* front;
        Node* rear;
        int queueLength;
    public:
        Queue();
        [[nodiscard]] bool isEmpty() const;
        [[nodiscard]] int getLength() const;
        [[nodiscard]] Node* getFront() const;
        [[nodiscard]] std::string getFrontPatientID() const;
        [[nodiscard]] std::string  getRearPatientID() const;
        bool enqueue(std::string Patient_id, std::string Condition);
        bool enqueue(std::string Patient_id, std::string Condition, std::string Admission);
        bool dequeue();
        bool inQueue(std::string ID);
        int getPatientQueueNumber(std::string ID);
};

#endif