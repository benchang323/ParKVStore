// value_stack.h

// Guards
#ifndef VALUE_STACK_H
#define VALUE_STACK_H

// Headers
#include <vector>
#include <string>

class ValueStack {
  private:
    // Vector to store the stack
    std::vector<std::string> stack;
    int stsize;
  public:
    // Constructor
    ValueStack();

    // Destructor
    ~ValueStack();

    // Member functions

    // Check if stack is empty
    // Parameters:
    //   void
    // Returns:
    //   bool - true if stack is empty, false otherwise
    bool is_empty() const;

    // Push a value onto the stack
    // Parameters:
    //   value - string to push onto the stack
    // Returns:
    //   void
    void push( const std::string &value );

    // Note: get_top() and pop() should throw OperationException
    // if called when the stack is empty
    // Get the top value of the stack
    // Parameters:
    //   void
    // Returns:
    //   std::string - top value of the stack
    std::string get_top() const;

    // Pop the top value of the stack
    // Parameters:
    //   void
    // Returns:
    //   void
    void pop();
    
    // Get the size of the stack
    // Parameters:
    //   void
    // Returns:
    //   int - size of the stack
    int size();
};

// End of include guard
#endif // VALUE_STACK_H
