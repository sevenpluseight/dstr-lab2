#include "supply_usage_log_dynamic_array.hpp"

SupplyUsageLogDynamicArray::SupplyUsageLogDynamicArray() {
    size = 0;
    capacity = 1;
    array = new SupplyUsageLog[capacity];
}

SupplyUsageLogDynamicArray::~SupplyUsageLogDynamicArray() {
    delete[] array;
}

int SupplyUsageLogDynamicArray::getSize() {
    return size;
}

void SupplyUsageLogDynamicArray::growArray() {
    capacity = capacity * 2;
    SupplyUsageLog* newArray = new SupplyUsageLog[capacity];
    for (int i = 0; i < size; i++) {
        newArray[i] = array[i];
    }
    delete[] array;
    array = newArray;
}

void SupplyUsageLogDynamicArray::appendArray(SupplyUsageLog log) {
    if (capacity == size) {
        growArray();
    }
    array[size] = log;
    size++;
}

SupplyUsageLog SupplyUsageLogDynamicArray::getElementAt(int index) {
    return array[index];
}

void SupplyUsageLogDynamicArray::setElementAt(int index, SupplyUsageLog value) {
    if (index >= 0 && index < size) {
        array[index] = value;
    }
}
