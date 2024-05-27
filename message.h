// message.h

// Guards
#ifndef MESSAGE_H
#define MESSAGE_H

// Headers
#include <vector>
#include <string>

// Enumeration for message types
enum class MessageType {
  // Used only for uninitialized Message objects
  NONE,

  // Requests
  LOGIN,
  CREATE,
  PUSH,
  POP,
  TOP,
  SET,
  GET,
  ADD,
  SUB,
  MUL,
  DIV,
  BEGIN,
  COMMIT,
  BYE,

  // Responses
  OK,
  FAILED,
  ERROR,
  DATA,
};

class Message {
private:
  // Member variables
  // Message type
  MessageType m_message_type;
  // Vector to store the arguments
  std::vector<std::string> m_args;

public:
  // Maximum encoded message length (including terminator newline character)
  static const unsigned MAX_ENCODED_LEN = 1024;

  // Constructor
  Message();

  // Constructor with message type and arguments
  Message( MessageType message_type, std::initializer_list<std::string> args = std::initializer_list<std::string>() );
  
  // Copy constructor
  Message( const Message &other );

  // Destructor
  ~Message();

  // Member functions

  // Assignment operator
  // Parameters:
  //   rhs - Message object to assign
  // Returns:
  //   Message object
  Message &operator=( const Message &rhs );

  // Get message type
  // Parameters:
  //   void
  // Returns:
  //   MessageType - message type
  MessageType get_message_type() const;

  // set_message_type: Sets the message type of the message.
  // Parameters:
  //   message_type - MessageType to set for the message
  // Returns:
  //   void
  void set_message_type( MessageType message_type );

  // get_username: Retrieves the username from the message arguments, typically the first argument.
  // Parameters:
  //   None
  // Returns:
  //   std::string - The username string
  std::string get_username() const;

  // get_table: Retrieves the table name from the message arguments, typically used in database contexts.
  // Parameters:
  //   None
  // Returns:
  //   std::string - The table name string
  std::string get_table() const;

  // get_key: Retrieves the key from the message arguments, often used in key-value store operations.
  // Parameters:
  //   None
  // Returns:
  //   std::string - The key string
  std::string get_key() const;

  // get_value: Retrieves the value from the message arguments, often used in operations that modify data.
  // Parameters:
  //   None
  // Returns:
  //   std::string - The value string
  std::string get_value() const;

  // get_quoted_text: Retrieves a quoted text from the message arguments, used in commands that involve text processing.
  // Parameters:
  //   None
  // Returns:
  //   std::string - The quoted text string
  std::string get_quoted_text() const;

  // push_arg: Adds an argument to the message's argument list.
  // Parameters:
  //   arg - The string argument to add
  // Returns:
  //   void
  void push_arg( const std::string &arg );

  // is_valid: Checks if the current message is properly formed according to its type and argument requirements.
  // Parameters:
  //   None
  // Returns:
  //   bool - True if the message is valid, false otherwise
  bool is_valid() const;

  // get_num_args: Retrieves the number of arguments in the message.
  // Parameters:
  //   None
  // Returns:
  //   unsigned - The number of arguments
  unsigned get_num_args() const { return m_args.size(); }

  // get_arg: Retrieves an argument from the message at the specified index.
  // Parameters:
  //   i - The index of the argument to retrieve
  // Returns:
  //   std::string - The argument at the specified index
  std::string get_arg( unsigned i ) const { return m_args.at( i ); }

  // single_id_check: Validates if a single identifier in the message is correctly formatted.
  // Parameters:
  //   None
  // Returns:
  //   bool - True if the identifier is valid, false otherwise
  bool single_id_check() const;

  // is_alpha: Checks if the given character is an alphabetic letter.
  // Parameters:
  //   word - The character to check
  // Returns:
  //   bool - True if the character is alphabetic, false otherwise
  bool is_alpha(char word) const;

  // is_valid_body: Validates the body of a string to ensure it contains only valid identifier characters.
  // Parameters:
  //   word - The string to validate
  // Returns:
  //   bool - True if the string body is valid, false otherwise
  bool is_valid_body(std::string word) const;

  // val_check: Validates the value in the message to ensure it meets specific formatting or content criteria.
  // Parameters:
  //   None
  // Returns:
  //   bool - True if the value is valid, false otherwise
  bool val_check() const;

  // quoted_text_check: Validates if the quoted text in the message is properly formatted.
  // Parameters:
  //   None
  // Returns:
  //   bool - True if the quoted text is valid, false otherwise
  bool quoted_text_check() const;

  // double_id_check: Validates if two identifiers in the message are correctly formatted.
  // Parameters:
  //   None
  // Returns:
  //   bool - True if both identifiers are valid, false otherwise
  bool double_id_check() const;

  bool is_identifier(const std::string &arg) const;
};


// End Guards
#endif // MESSAGE_H










