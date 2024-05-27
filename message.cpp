// message.cpp

// Headers
#include <set>
#include <map>
#include <regex>
#include <cassert>
#include <string>
#include "message.h"
#include <iostream>

// Constructor
using std::string;
using std::cout;
using std::endl;

// Constructor
Message::Message()
  : m_message_type(MessageType::NONE)
{
}

// Constructor with message type and arguments
Message::Message( MessageType message_type, std::initializer_list<std::string> args )
  : m_message_type( message_type )
  , m_args( args )
{
}

// Copy constructor
Message::Message( const Message &other )
  : m_message_type( other.m_message_type )
  , m_args( other.m_args )
{
}

// Destructor
Message::~Message()
{
}

// Assignment operator
// Parameters:
//   rhs - Message object to assign
// Returns:
//   Message object
Message &Message::operator=( const Message &rhs )
{
  // Check for self-assignment
  this->m_message_type = rhs.m_message_type;
  this->m_args = rhs.m_args;

  // Return reference to this object
  return *this;
}

// Get message type
// Parameters:
//   void
// Returns:
//   MessageType - message type
MessageType Message::get_message_type() const
{
  return m_message_type;
}

// set_message_type: Sets the message type of the message.
// Parameters:
//   message_type - MessageType to set for the message
// Returns:
//   void
void Message::set_message_type(MessageType message_type)
{
  m_message_type = message_type;
}

// get_username: Retrieves the username from the message arguments, typically the first argument.
// Parameters:
//   None
// Returns:
//   std::string - The username string
std::string Message::get_username() const
{
  return m_args[0];
}

// get_table: Retrieves the table name from the message arguments, typically used in database contexts.
// Parameters:
//   None
// Returns:
//   std::string - The table name string
std::string Message::get_table() const
{
  return m_args[0];
}

// get_key: Retrieves the key from the message arguments, often used in key-value store operations.
// Parameters:
//   None
// Returns:
//   std::string - The key string
std::string Message::get_key() const
{
  return m_args[1];
}

// get_value: Retrieves the value from the message arguments, often used in operations that modify data.
// Parameters:
//   None
// Returns:
//   std::string - The value string
std::string Message::get_value() const
{
  return m_args[0];
}

// get_quoted_text: Retrieves a quoted text from the message arguments, used in commands that involve text processing.
// Parameters:
//   None
// Returns:
//   std::string - The quoted text string
std::string Message::get_quoted_text() const
{
  return m_args[0];
}

// push_arg: Adds an argument to the message's argument list.
// Parameters:
//   arg - The string argument to add
// Returns:
//   void
void Message::push_arg( const std::string &arg )
{
  m_args.push_back( arg );
}

// is_valid: Checks if the current message is properly formed according to its type and argument requirements.
// Parameters:
//   None
// Returns:
//   bool - True if the message is valid, false otherwise
bool Message::is_valid() const
{
  // Message type
  int m_type = static_cast<int> (m_message_type);

  // 1 identifier argument
  if ((m_type == 1 || m_type == 2)  &&  m_args.size() == 1 && single_id_check()) {
    return true;
  }

  // No arguments
  if ((m_type == 4 || m_type == 5 || (m_type >= 8 && m_type <= 15))  && m_args.size() == 0){
    return true;
  }

  // value arguments
  if ((m_type == 3 || m_type == 18) && m_args.size() == 1 && val_check()) {
    return true;
  }

  // quoted text arguments
  if ((m_type == 16 || m_type == 17) && m_args.size()) {
    return true;
  }
  
  // 2 identifier arguments
  if ((m_type == 6 || m_type == 7) && m_args.size() == 2 && double_id_check()) {
    return true;
  }

  return false;
}

// val_check: Validates the value in the message to ensure it meets specific formatting or content criteria.
// Parameters:
//   None
// Returns:
//   bool - True if the value is valid, false otherwise
bool Message::val_check() const {
  // Check if the value is empty
  string value = m_args[0];
  if (value.length() == 0) {
    return false;
  }

  // Iterate through the value
  for (int i = 0; i < value.length(); ++i) {
    // Check if the value is a number
    if (i > 0 && value[i] == ' ') {
      return false;
    }
  }
  return true;
}

// single_id_check: Validates if a single identifier in the message is correctly formatted.
// Parameters:
//   None
// Returns:
//   bool - True if the identifier is valid, false otherwise
bool Message::single_id_check() const {
  // Check if the identifier is empty
  if (m_args[0].length() == 0) {
    return false;
  }

  // Check if the identifier is valid
  if (!is_alpha(m_args[0][0]) || !is_valid_body(m_args[0])) {
    return false;
  }

  return true;
}

// double_id_check: Validates if two identifiers in the message are correctly formatted.
// Parameters:
//   None
// Returns:
//   bool - True if both identifiers are valid, false otherwise
bool Message::double_id_check() const {
  // Check if the identifiers are empty
  if (m_args[0].length() == 0 || m_args[1].length() == 0) {
    return false;
  }

  // Check if the identifiers are valid
  if (!is_alpha(m_args[0][0]) || !is_valid_body(m_args[0]) || !isalpha(m_args[1][0]) || !is_valid_body(m_args[1])) {
    return false;
  }
  return true;
}

// is_alpha: Checks if the given character is an alphabetic letter.
// Parameters:
//   word - The character to check
// Returns:
//   bool - True if the character is alphabetic, false otherwise
bool Message::is_alpha(char c) const {
  // Check if the character is alphabetic
  if ((c >= 'a' && c <= 'z') || ( c >= 'A' && c <= 'Z')) {
    return true;
  }

  return false;
}

// is_valid_body: Validates the body of a string to ensure it contains only valid identifier characters.
// Parameters:
//   word - The string to validate
// Returns:
//   bool - True if the string body is valid, false otherwise
bool Message::is_valid_body(string word) const {
  // Iterate through the string
  for (const char &c : word) {
    // Check if the character is valid
    if (!is_alpha(c) && c != '_' && (c < '0' && c > '9')) {
      return false;
    }
  }
  return true;
}

bool Message::is_identifier(const string &arg) const {
  if (arg.empty() || !isalpha(arg[0])) return false;

  for (size_t i = 1; i < arg.size(); i++) {
    if (!isalpha(arg[i]) && !isdigit(arg[i]) && arg[i] != '_') 
      return false;
  }

  return true;
}