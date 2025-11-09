#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <string>

class DynamicArray {
    private:
        int size;
        int capacity;
        std::string* array;
    public:
        DynamicArray();
        DynamicArray(int inputCapacity);
        int getSize();
        int getCapacity();
        void growArray();
        void shrinkArray();
        void appendArray(std::string inputString);
        void popArray();
        void clearArray();
        bool isInArray(std::string text);
        std::string getElementAt(int index);
};

#endif