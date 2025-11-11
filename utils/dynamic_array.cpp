#include "dynamic_array.hpp"
#include <iostream>
#include <array>
// #include <array>

DynamicArray::DynamicArray() {
    size = 0;
    capacity = 1;
    array = new std::string[capacity];
}

DynamicArray::DynamicArray(int inputCapacity) {
    size = 0;
    capacity = inputCapacity;
    array = new std::string[capacity];
}

int DynamicArray::getSize() {
    return size;
}

int DynamicArray::getCapacity() {
    return capacity;
}

void DynamicArray::growArray() {
    capacity = capacity * 2;
    std::string* newArray = new std::string[capacity];
    for (int i = 0; i < size; i++) {
        newArray[i] = array[i];
    }
    delete[] array;
    array = newArray;
}

void DynamicArray::shrinkArray() {
    capacity = size;
    std::string* newArray = new std::string[capacity];
    for (int i = 0; i < capacity; i++) {
        newArray[i] = array [i];
    }
    delete[] array;
    array = newArray;
}

void DynamicArray::appendArray(std::string inputString) {
    if (capacity == size) {
        growArray();
    }
    if (inputString.length() > 0) {
        array[size] = inputString;
        size++;
    }
}

void DynamicArray::popArray() {
    array[size - 1] = "";
    size--;
    if (capacity == size) {
        shrinkArray();
    }
}

void DynamicArray::clearArray() {
    size = 0;
    capacity = 1;
    std::string* newArray = new std::string[capacity];
    delete[] array;
    array = newArray;
}

bool DynamicArray::isInArray(std::string text) {
    for(int i = 0; i < size; i++) {
        if (array[i] == text) {
            return true;
        }
    }
    return false;
}

std::string DynamicArray::getElementAt(int index) {
    return array[index];
}

