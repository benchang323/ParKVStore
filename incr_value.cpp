// incr_value.cpp

// Headers
#include <iostream>
#include <string>
#include "csapp.h"
#include "client_helper.h"

// Main function
int main(int argc, char **argv) {
  // Check for correct number of arguments
  if ( argc != 6 && (argc != 7 || std::string(argv[1]) != "-t") ) {
    std::cerr << "Usage: ./incr_value [-t] <hostname> <port> <username> <table> <key>\n";
    std::cerr << "Options:\n";
    std::cerr << "  -t      execute the increment as a transaction\n";
    return 1;
  }

  // Count for transaction
  int count = 1;

  // Check if transaction is used
  bool use_transaction = false;

  // Check if transaction is used
  if ( argc == 7 ) {
    use_transaction = true;
    count = 2;
  }

  // Extract arguments
  std::string hostname = argv[count++];
  std::string port = argv[count++];
  std::string username = argv[count++];
  std::string table = argv[count++];
  std::string key = argv[count++];

  // Establish connection using hostname and port
  int fd = open_clientfd(hostname.c_str(), port.c_str());
  if (fd < 0) { 
    std::cerr << "Error: Couldn't connect to server" << std::endl;
    exit(1);
  }

  // Login using username
  if (!login(username, fd)) {
    exit(1);
  }

  // Check if transaction is used, begin the transaction
  if (use_transaction) {
    if (!begin(fd)) {
      exit(1);
    }
  }
  
  // Retrieve value from table with key
  if (!get(table, key, fd)) {
    exit(1);
  }

  // Push the integer 1 to the stack
  if (!push("1", fd)) {
    exit(1);
  }

  // Add the top two elements of the stack
  if (!add(fd)) {
    exit(1);
  }

  // Set the value of the key in the table
  if (!set(table, key, fd)) {
    exit(1);
  }

  // If transaction is used, commit the transaction
  if (use_transaction) {
    if(!commit(fd)) {
      exit(1);
    }
  }

  // Send BYE to server
  std::string bye = "BYE\n";
  rio_writen(fd, bye.c_str(), bye.size());

  // Close connection
  close(fd);

  // Successful exit
  return 0;
}
