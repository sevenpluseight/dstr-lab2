#ifndef STACK_HPP
#define STACK_HPP

#include <iostream>

#include "entities.hpp"

/**
 * @brief Implements a stack data structure for managing medical supplies
 *
 * @note
 * - Uses a static array
 * - Follows LIFO (Last In, First Out) principle
 */

#define MAX_STACK_CAPACITY 1000

class SupplyStack {
private:
    Supply supplies[MAX_STACK_CAPACITY]; /// Static array to hold supply items
    int top; /// Index of the top element in the stack

public:
    /**
     * @brief Constructor to initialize an empty stack
     */
    SupplyStack();

    /**
     * @brief Checks if the stack is full
     * @return true if stack is full, otherwise false
     */
    bool isStackFull() const;

    /**
     * @brief Checks if the stack is empty
     * @return true if stack is empty, otherwise false
     */
    bool isStackEmpty() const;

    /**
     * @brief Push a new supply onto the stack
     * @param newSupply The supply item to be added
     */
    void push(const Supply& newSupply);

    /**
     * @brief Peek at the last added supply without removing it
     * @return The top supply item
     */
    Supply peek() const;

    /**
     * @brief Pop (remove) the last added supply from the stack
     * @return The removed supply item
     */
    Supply pop();

    // /**
    //  * @brief Displays all supplies in the stack (for debugging or viewing)
    //  */
    // void display() const;

    /**
     * @brief Get the current top index of the stack
     * @return Index of the top supply
     */
    int getTopIndex() const;

    /**
     * @brief Get the supply at a specific index
     * @param index Index from 0 to top
     * @return Supply at the given index
     */
    Supply getSupplyAt(int index) const;
};

#endif
