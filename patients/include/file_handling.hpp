#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "patient_array.hpp"
#include "dynamic_array.hpp"
#include "queue.hpp"
#include "string_utils.hpp"
#include "config.hpp"

inline std::string getCurrentTimeString(std::time_t t) {
    std::tm tmStruct;

#ifdef OS_WINDOWS
    localtime_s(&tmStruct, &t);  // Windows thread-safe
#else
    localtime_r(&t, &tmStruct);  // POSIX thread-safe
#endif

    std::stringstream ss;
    ss << std::put_time(&tmStruct, "%a %b %d %H:%M:%S %Y"); // mimic ctime() format
    return ss.str();
}

inline PatientArray readPatientFile(PatientArray array, const std::string& filepath) {
    array.clearArray();
    std::ifstream patientFile(filepath);

    if (!patientFile.is_open()) {
        MessageHandler::info("File \"" + filepath + "\" cannot be found. \n");
    } else {
        std::string header;
        getline(patientFile, header, '\n');
        header.erase(0, header.size() - 1);

        DynamicArray patientElements;
        std::string readline;
        while (getline(patientFile, readline, '\n')) {
            patientElements = split(readline, ',', patientElements);
            if (auto patient = Patient(patientElements);
                !patient.patient_id.empty()) {
                array.appendArray(patient);
            }
            patientElements.clearArray();
        }
        patientFile.close();
        patientElements.clearArray();
    }
    return array;
}

inline Queue readQueueFile(Queue queue, const std::string& filepath) {
    std::ifstream queueFile(filepath);

    if (!queueFile.is_open()) {
        MessageHandler::info("File \"" + filepath + "\" cannot be found. \n");
    } else {
        std::string header;
        getline(queueFile, header, '\n');
        header.erase(0, header.size() - 1);

        DynamicArray queueElements;
        std::string readline;
        while (getline(queueFile, readline, '\n')) {
            queueElements = split(readline, ',', queueElements);

            const std::string patient_id = queueElements.getElementAt(0);
            const std::string condition = queueElements.getElementAt(1);
            const std::string admission = queueElements.getElementAt(2);
            const PatientStatus status = parsePatientStatus(trim(queueElements.getElementAt(3)));
            if (status == ADMITTED) {
                queue.enqueue(patient_id, condition, admission);
            }
            queueElements.clearArray();
        }
        queueFile.close();
    }
    return queue;
}

inline void appendPatientFile(const std::string& newLine, const std::string& filepath) {
    std::ifstream patientFile(filepath);
    if (!patientFile.is_open()) {
        MessageHandler::info("File \"" + filepath + "\" cannot be found. \n");
        return;
    }

    std::string readline;
    DynamicArray originalLines;
    while (getline(patientFile, readline, '\n')) {
        originalLines.appendArray(readline);
    }
    patientFile.close();

    std::ofstream outputFile(filepath);
    for (int i = 0; i < originalLines.getSize(); i++) {
        outputFile << originalLines.getElementAt(i) + "\n";
    }
    outputFile << newLine + "\n";
    outputFile.close();
}

inline void modifyPatientFile(const std::string& ID, const std::string& newLine, const std::string& filepath) {
    std::ifstream patientFile(filepath);
    if (!patientFile.is_open()) {
        MessageHandler::info("File \"" + filepath + "\" cannot be found. \n");
        return;
    }

    std::string readline;
    DynamicArray originalLines;
    while (getline(patientFile, readline, '\n')) {
        originalLines.appendArray(readline);
    }
    patientFile.close();

    std::ofstream outputFile(filepath);
    for (int i = 0; i < originalLines.getSize(); i++) {
        std::string line = originalLines.getElementAt(i);
        DynamicArray patientElements;
        patientElements = split(line, ',', patientElements);

        if (patientElements.getElementAt(0) == ID) {
            outputFile << newLine + "\n";
        } else {
            outputFile << line + "\n";
        }
    }
    outputFile.close();
}

inline void enqueueQueueFile(const std::string& PatientID, std::string condition, const std::string& filepath) {
    std::ifstream queueFile(filepath);
    if (!queueFile.is_open()) {
        MessageHandler::info("File \"" + filepath + "\" cannot be found. \n");
        return;
    }

    std::string readline;
    DynamicArray originalLines;
    while (getline(queueFile, readline, '\n')) {
        originalLines.appendArray(readline);
    }
    queueFile.close();

    std::ofstream outputFile(filepath);
    for (int i = 0; i < originalLines.getSize(); i++) {
        outputFile << originalLines.getElementAt(i) + "\n";
    }

    auto admission_time = std::chrono::system_clock::now();
    std::time_t formatted_admission_time = std::chrono::system_clock::to_time_t(admission_time);

    outputFile << PatientID + "," + condition + "," + getCurrentTimeString(formatted_admission_time) + "," +
        patientStatusToString(ADMITTED) + "," + "NOT_DISCHARGED\n";

    outputFile.close();
}

inline void dequeueQueueFile(const std::string& filepath) {
    bool dequeued = false;

    std::ifstream queueFile(filepath);
    if (!queueFile.is_open()) {
        MessageHandler::info("File \"" + filepath + "\" cannot be found. \n");
        return;
    }

    std::string readline;
    DynamicArray originalLines;
    while (getline(queueFile, readline, '\n')) {
        originalLines.appendArray(readline);
    }
    queueFile.close();

    std::ofstream outputFile(filepath);
    for (int i = 0; i < originalLines.getSize(); i++) {
        std::string line = originalLines.getElementAt(i);
        DynamicArray queueElements;
        queueElements = split(line, ',', queueElements);

        if (!dequeued && parsePatientStatus(queueElements.getElementAt(3)) == ADMITTED) {
            auto discharge_time = std::chrono::system_clock::now();
            std::time_t formatted_discharge_time = std::chrono::system_clock::to_time_t(discharge_time);

            outputFile << queueElements.getElementAt(0) + "," + queueElements.getElementAt(1) + "," +
                queueElements.getElementAt(2) + "," + patientStatusToString(DISCHARGED) + "," +
                getCurrentTimeString(formatted_discharge_time) + "\n";

            dequeued = true;
        } else {
            outputFile << line + "\n";
        }
    }
    outputFile.close();
}

#endif

// #ifndef FILE_HANDLING_H
// #define FILE_HANDLING_H
//
// #include <fstream>
// #include <chrono>
// #include <ctime>
// #include "patient_array.hpp"
// #include "dynamic_array.hpp"
// #include "queue.hpp"
// #include "string_utils.hpp"
//
// inline PatientArray readPatientFile(PatientArray array, const std::string& filepath) {
//     array.clearArray();
//     std::ifstream patientFile(filepath);
//
//     if(!patientFile.is_open()) {
//         MessageHandler::info("File \"" + filepath + "\" cannot be found. \n");
//     }
//     else {
//         std::string header;
//         getline(patientFile, header, '\n');
//         header.erase(0, header.size() - 1);
//
//         DynamicArray patientElements;
//         std::string readline;
//         // int invalidCount = 0;
//         while(getline(patientFile, readline, '\n')) {
//
//             patientElements = split(readline, ',', patientElements);
//             if (auto patient = Patient(patientElements);
//                 !patient.patient_id.empty()) {
//                 array.appendArray(patient);
//             }
//             else {
//                 // invalidCount++;
//             }
//             patientElements.clearArray();
//         }
//         patientFile.close();
//         patientElements.clearArray();
//         // MessageHandler::info("Invalid patient record count: " + std::to_string(invalidCount));
//         // MessageHandler::info("Patient records loaded. \n");
//     }
//     return array;
// }
//
// inline Queue readQueueFile(Queue queue, const std::string& filepath) {
//     std::ifstream queueFile(filepath);
//
//     if(!queueFile.is_open()) {
//         MessageHandler::info("File \"" + filepath + "\" cannot be found. \n");
//     }
//     else {
//         std::string header;
//         getline(queueFile, header, '\n');
//         header.erase(0, header.size() - 1);
//
//         DynamicArray queueElements;
//         std::string readline;
//         // int invalidCount = 0;
//         while(getline(queueFile, readline, '\n')) {
//
//             queueElements = split(readline, ',', queueElements);
//
//             const std::string patient_id = queueElements.getElementAt(0);
//             const std::string condition = queueElements.getElementAt(1);
//             const std::string admission = queueElements.getElementAt(2);
//             const PatientStatus status = parsePatientStatus(trim(queueElements.getElementAt(3)));
//             if (status == ADMITTED) {
//                 queue.enqueue(patient_id, condition, admission);
//             }
//             // else if (status == NONE) {
//             //     invalidCount++;
//             // }
//             queueElements.clearArray();
//         }
//         queueFile.close();
//         // MessageHandler::info("Invalid queue record count: " + std::to_string(invalidCount));
//         // MessageHandler::info("Queue records loaded. \n");
//     }
//     return queue;
// }
//
// inline void appendPatientFile(const std::string& newLine, const std::string& filepath) {
//
//     std::ifstream patientFile(filepath);
//     if(!patientFile.is_open()) {
//         MessageHandler::info("File \"" + filepath + "\" cannot be found. \n");
//         return;
//     }
//     std::string readline;
//
//     DynamicArray originalLines;
//     while(getline(patientFile, readline, '\n')) {
//         originalLines.appendArray(readline);
//     }
//     patientFile.close();
//
//     std::ofstream outputFile;
//     outputFile.open (filepath);
//
//     for(int i = 0; i < originalLines.getSize(); i++) {
//         std::string line = originalLines.getElementAt(i);
//         outputFile << line + "\n";
//     }
//     outputFile << newLine + "\n";
//     outputFile.close();
//     fflush(stdin);
// }
//
// inline void modifyPatientFile(const std::string& ID, const std::string& newLine, const std::string& filepath) {
//     std::ifstream patientFile(filepath);
//     if(!patientFile.is_open()) {
//         MessageHandler::info("File \"" + filepath + "\" cannot be found. \n");
//         return;
//     }
//     std::string readline;
//
//     DynamicArray originalLines;
//     while(getline(patientFile, readline, '\n')) {
//         originalLines.appendArray(readline);
//     }
//     patientFile.close();
//
//     std::ofstream outputFile;
//     outputFile.open (filepath);
//
//     for(int i = 0; i < originalLines.getSize(); i++) {
//         std::string line = originalLines.getElementAt(i);
//         DynamicArray patientElements;
//         patientElements = split(line, ',', patientElements);
//         if(patientElements.getElementAt(0) == ID) {
//             outputFile << newLine + "\n";
//         }
//         else {
//             outputFile << line + "\n";
//         }
//     }
//     outputFile.close();
// }
//
// inline void enqueueQueueFile(const std::string& PatientID, std::string condition, const std::string& filepath) {
//
//     std::ifstream queueFile(filepath);
//     if(!queueFile.is_open()) {
//         MessageHandler::info("File \"" + filepath + "\" cannot be found. \n");
//         return;
//     }
//     std::string readline;
//
//     DynamicArray originalLines;
//     while(getline(queueFile, readline, '\n')) {
//         originalLines.appendArray(readline);
//     }
//     queueFile.close();
//
//     std::ofstream outputFile;
//     outputFile.open (filepath);
//
//     for(int i = 0; i < originalLines.getSize(); i++) {
//         std::string line = originalLines.getElementAt(i);
//         outputFile << line + "\n";
//     }
//
//     auto admission_time_in_seconds = std::chrono::system_clock::now();
//     std::time_t formatted_admission_time = std::chrono::system_clock::to_time_t(admission_time_in_seconds);
//
//     outputFile << PatientID + "," + condition + "," + trim(std::ctime(&formatted_admission_time)) + "," +
//         patientStatusToString(ADMITTED) + "," + "NOT_DISCHARGED\n";
//
//     outputFile.close();
//     fflush(stdin);
// }
//
// inline void dequeueQueueFile(const std::string& filepath) {
//
//     bool dequeued = false;
//
//     std::ifstream queueFile(filepath);
//     if(!queueFile.is_open()) {
//         MessageHandler::info("File \"" + filepath + "\" cannot be found. \n");
//         return;
//     }
//     std::string readline;
//
//     DynamicArray originalLines;
//     while(getline(queueFile, readline, '\n')) {
//         originalLines.appendArray(readline);
//     }
//     queueFile.close();
//
//     std::ofstream outputFile;
//     outputFile.open (filepath);
//
//     for(int i = 0; i < originalLines.getSize(); i++) {
//         std::string line = originalLines.getElementAt(i);
//         DynamicArray queueElements;
//         queueElements = split(line, ',', queueElements);
//         if(!dequeued && parsePatientStatus(queueElements.getElementAt(3)) == ADMITTED) {
//
//             auto discharge_time_in_seconds = std::chrono::system_clock::now();
//             std::time_t formatted_discharged_time = std::chrono::system_clock::to_time_t(discharge_time_in_seconds);
//
//             outputFile << queueElements.getElementAt(0) + "," + queueElements.getElementAt(1) + "," + queueElements.getElementAt(2) + "," +
//                 patientStatusToString(DISCHARGED) + "," + trim(std::ctime(&formatted_discharged_time)) + "\n";
//
//             dequeued = true;
//         }
//         else {
//             outputFile << line + "\n";
//         }
//         fflush(stdin);
//     }
//     fflush(stdin);
//     outputFile.close();
// }
//
// #endif