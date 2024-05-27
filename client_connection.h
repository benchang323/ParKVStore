// Guard
#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

// Headers
#include <set>
#include "message.h"
#include "csapp.h"

// Forward declarations
class Server; 
class Table; 

class ClientConnection {
private:
  // Member variables
  // Server object
  Server *m_server;
  // Client file descriptor
  int m_client_fd;
  // Buffer for reading from the client
  rio_t m_fdbuf;
  // Variable to keep track of the transaction status
  bool inTransaction;
  // Stack to manage values
  std::stack<std::string> value_stack;  
  // To keep track of locked tables during a transaction
  std::unordered_map<Table*, bool> lockedTables;   

  // copy constructor and assignment operator are prohibited

  // copy constructor
  ClientConnection( const ClientConnection & );
  // assignment operator
  ClientConnection &operator=( const ClientConnection & );

public:
  // Constructor
  ClientConnection( Server *server, int client_fd );
  // Destructor
  ~ClientConnection();

  // The method checks if the operands are valid
  // Parameters:
  //   left - left operand
  //   right - right operand
  // Returns:
  //   true if both operands are valid, false otherwise
  bool check_operands(std::string left, std::string right);

  // This method initializes the client connection that allows the client to chat with the server
  // Parameters:
  //   none
  // Returns:
  //   void
  void chat_with_client();

  // This method sends a response to the client
  // Parameters:
  //   message - message to send
  // Returns:
  //   void
  void send_response(const Message& msg);

  // This method rolls back all the changes made during a transaction
  // Parameters:
  //   none
  // Returns:
  //   void
  void roll_back_all();
  
  // This function pushes a value to the stack
  // Parameters:
  //  table - pointer to the table
  // Returns:
  //  void
  void push_value(const std::string& value);

  // This function pops a value from the stack
  // Parameters:
  //  none
  // Returns:
  //  std::string - value
  std::string pop_value();

  // This function returns the top value from the stack
  // Parameters:
  //  none
  // Returns:
  //  std::string - value
  std::string top_value();

  // This function begins a transaction
  // Parameters:
  //  none
  // Returns:
  //  void
  void begin_transaction();

  // This function commits a transaction
  // Parameters:
  //  none
  // Returns:
  //  void
  void commit_transaction();

  // This function sets a value in the table
  // Parameters:
  //  table - table name
  //  key - key
  //  value - value
  // Returns:
  //  void
  void set_value(const std::string& table, const std::string& key, const std::string& value);

  // This function gets a value from the table
  // Parameters:
  //  table - table name
  //  key - key
  // Returns:
  //  std::string - value
  std::string get_value(const std::string& table, const std::string& key);
};

#endif // CLIENT_CONNECTION_H