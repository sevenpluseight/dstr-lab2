#include "stack.hpp"
#include <message_handler.hpp>

/**
 * @brief Constructor initializes an empty stack
 */
SupplyStack::SupplyStack() : top(-1) {}

/**
 * @brief Checks if the stack is full
 * @return true if full, false otherwise
 */
bool SupplyStack::isStackFull() const {
    return top >= MAX_STACK_CAPACITY - 1;
}

/**
 * @brief Checks if the stack is empty
 * @return true if empty, false otherwise
 */
bool SupplyStack::isStackEmpty() const {
    return top == -1;
}

/**
 * @brief Push a new supply onto the stack
 * @param newSupply Supply to push
 */
void SupplyStack::push(const Supply &newSupply) {
    if (isStackFull()) {
        MessageHandler::error("Cannot add supply. The storage is full"); // Stack is full
        return;
    }
    top++;
    supplies[top] = newSupply;
}

/**
 * @brief Peek at the last added supply without removing it
 * @return The top supply
 */
Supply SupplyStack::peek() const {
    if (isStackEmpty()) {
        MessageHandler::error("Storage is empty."); // Stack is empty, cannot peek
        return Supply{};
    }
    return supplies[top];
}

/**
 * @brief Pop the last added supply from the stack
 * @return The removed supply
 */
Supply SupplyStack::pop() {
    if (isStackEmpty()) {
        MessageHandler::error("Storage is empty."); // Stack is empty, cannot pop
        return Supply{};
    }
    Supply removed = supplies[top];
    top--;
    return removed;
}

// /**
//  * @brief Display all supplies in the stack
//  */
// void SupplyStack::display() const {
//
// }

/**
 * @brief Get the current top index of the stack
 * @details Returns the index of the topmost element
 * @return Index of the top supply, -1 if the stack is empty
 *
 */
int SupplyStack::getTopIndex() const {
    return top;
}

/**
 * @brief Get the supply at a specific index
 * @details Provides read-only access to the supply at the given index
 * @param index Index of the supply (0 to top)
 * @return Supply at the given index
 * @throws std::out_of_range if index is invalid
 */
Supply SupplyStack::getSupplyAt(int index) const {
    if (index >= 0 && index <= top) return supplies[index];
    throw std::out_of_range("Index out of bounds in SupplyStack");
}
