#ifndef CLIENT_HELPER_H
#define CLIENT_HELPER_H

#include "csapp.h"
#include <string>

// Attempts to log the user in to the server.
// Parameters:
//   username - username to login
//   fd - file descriptor of the server connection
// Returns:
//   true if login successful, else false
bool login(std::string username, int fd);

// Sends a GET request to the server to retrieve a value associated with a key from table.
// Parameters:
//   table - name of table
//   key - key whose value is to be 
//   fd - file descriptor of the server connection
// Returns:
//   true if request successful, else false
bool get(std::string table, std::string key, int fd);

// Retrieves the top value from the server's response.
// Parameters:
//   fd - file descriptor of the server connection
// Returns:
//   true if top value retrieved successfully, else false
bool top(int fd);

// Pushes a value onto the server's operand stack.
// Parameters:
//   value - value to push
//   fd - file descriptor of the server connection
// Returns:
//   true if push successful, else false
bool push(std::string value, int fd);

// Sets a value in a table for a given key.
// Parameters:
//   table - name of table
//   key - key to set the value for
//   fd - file descriptor of the server connection
// Returns:
//   true if set operation was successful, else false
bool set(std::string table, std::string key, int fd);

// Begins a transaction on the server.
// Parameters:
//   fd - file descriptor of server connection
// Returns:
//   true if transaction was successfully started, else false
bool begin(int fd);

// Commits current transaction on the server.
// Parameters:
//   fd - file descriptor of the server connection
// Returns:
//   true if the transaction was successfully committed, else false
bool commit(int fd);

// Adds two top values from the server's operand stack.
// Parameters:
//   fd - file descriptor of the server connection
// Returns:
//   true if the add operation was successful, else false
bool add(int fd);

// Prints the retrieved value to the console.
// Parameters:
//   response - server response containing the value
void print_value(std::string response);

// Check for errors in server responses
// Parameters:
//   response - server response as a C-style string
//   fd - file descriptor of the server connection
// Returns:
//   true if there was an error in the response, false otherwise
bool check_error(std::string response, int fd);

// Check for errors specifically for the TOP command
// Parameters:
//   response - server response as a C-style string
//   fd - file descriptor of the server connection
// Returns:
//   true if there was an error or an unexpected "OK" response, false otherwise
bool check_error_top(std::string response, int fd);

#endif //CLIENT_HELPER_H
