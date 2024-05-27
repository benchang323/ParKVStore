// value_stack.cpp

// Headers
#include "value_stack.h"
#include "exceptions.h"

// Constructor
ValueStack::ValueStack()
  // TODO: initialize member variable(s) (if necessary)
{
}

// Destructor
ValueStack::~ValueStack()
{
}

// Check if stack is empty
// Parameters:
//   void
// Returns:
//   bool - true if stack is empty, false otherwise
bool ValueStack::is_empty() const
{
  // TODO: implement
  return stack.empty();
}

// Push a value onto the stack
// Parameters:
//   value - string to push onto the stack
// Returns:
//   void
void ValueStack::push( const std::string &value )
{
  // TODO: implement
  stsize++;
  stack.push_back(value);
}

// Note: get_top() and pop() should throw OperationException
// if called when the stack is empty
// Get the top value of the stack
// Parameters:
//   void
// Returns:
//   std::string - top value of the stack
std::string ValueStack::get_top() const
{
  // TODO: implement
  if (is_empty()) {
    throw OperationException("Cannot call top on an empty stack");
  }
  return stack.back();
}

// Pop the top value of the stack
// Parameters:
//   void
// Returns:
//   void
void ValueStack::pop()
{
  // TODO: implement
  if (stack.empty()) {
    throw OperationException("Cannot call pop on an empty stack");
  }
  stsize--;
  stack.pop_back();
}

// Get the size of the stack
// Parameters:
//   void
// Returns:
//   int - size of the stack
int ValueStack::size() {
  return stsize;
}