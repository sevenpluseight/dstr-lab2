#ifndef SUPPLY_USAGE_LOG_DYNAMIC_ARRAY_HPP
#define SUPPLY_USAGE_LOG_DYNAMIC_ARRAY_HPP

#include "entities.hpp"

class SupplyUsageLogDynamicArray {
    private:
        int size;
        int capacity;
        SupplyUsageLog* array;
        void growArray();
    public:
        SupplyUsageLogDynamicArray();
        ~SupplyUsageLogDynamicArray();
        int getSize();
        void appendArray(SupplyUsageLog log);
        SupplyUsageLog getElementAt(int index);
        void setElementAt(int index, SupplyUsageLog value);
};

#endif
