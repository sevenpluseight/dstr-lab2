#include <chrono>
#include <ctime>
#include <utility>
#include "queue.hpp"
#include "string_utils.hpp"
#include "message_handler.hpp"

Queue::Queue() {
    front = rear = nullptr;
    queueLength = 0;
}

bool Queue::isEmpty() const {
    return front == nullptr;
}

int Queue::getLength() const {
    return queueLength;
}

Node* Queue::getFront() const {
    return front;
}

std::string Queue::getFrontPatientID() const {
    if(isEmpty()) {
        return {};
    }
    return front->patient_id;
}

std::string Queue::getRearPatientID() const {
    if(isEmpty()) {
        return {};
    }
    return rear->patient_id;
}

bool Queue::enqueue(const std::string Patient_id, std::string Condition) {
    const auto newNode = new Node();

    newNode->patient_id = Patient_id;
    newNode->condition = Condition;
    newNode->discharge = "NOT_DISCHARGED";
    newNode->status = ADMITTED;
    newNode->next = nullptr;

    const auto admission_time_in_seconds = std::chrono::system_clock::now();
    const std::time_t formatted_admission_time = std::chrono::system_clock::to_time_t(admission_time_in_seconds);
    newNode->admission = trim(std::ctime(&formatted_admission_time));

    if (isEmpty()) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
    queueLength++;
    return true;
}

bool Queue::enqueue(const std::string Patient_id, std::string Condition, std::string Admission) {
    Node* newNode = new Node();

    newNode->patient_id = Patient_id;
    newNode->condition = Condition;
    newNode->discharge = "NOT_DISCHARGED";
    newNode->status = ADMITTED;
    newNode->next = nullptr;
    newNode->admission = Admission;

    if (isEmpty()) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
    queueLength++;
    return true;
}

bool Queue::dequeue() {
    if (isEmpty()) {
        MessageHandler::info("Cannot dequeue. Queue is empty.");
        return false;
    }
    front = front->next;
    if(front == nullptr) {
        rear = nullptr;
    }
    queueLength--;
    return true;
}

bool Queue::inQueue(std::string ID) {
    const Node* temp = front;
    for (int i = 1; i <= getLength(); i++) {
        if (temp->patient_id == ID) {
            delete temp;
            return true;
        }
        temp = temp->next;
    }
    delete temp;
    return false;
}

int Queue::getPatientQueueNumber(std::string ID) {
    Node* temp = front;
    for (int i = 1; i <= getLength(); i++) {
        if (temp->patient_id == ID) {
            delete temp;
            return i;
        }
        temp = temp->next;
    }
    delete temp;
    MessageHandler::info("Patient not found in queue.");
    return -1;
}
