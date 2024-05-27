// set_value.cpp

// Headers
#include <iostream>
#include "csapp.h"
#include "client_helper.h"
#include <string>

// Main function
int main(int argc, char **argv)
{
  // Check for correct number of arguments
  if (argc != 7) {
    std::cerr << "Usage: ./set_value <hostname> <port> <username> <table> <key> <value>\n";
    return 1;
  }

  // Get arguments
  std::string hostname = argv[1];
  std::string port = argv[2];
  std::string username = argv[3];
  std::string table = argv[4];
  std::string key = argv[5];
  std::string value = argv[6];
  
  // Connect to server
  int fd = open_clientfd(hostname.c_str(), port.c_str());
  if (fd < 0) { 
    std::cerr << "Error: Couldn't connect to server" << std::endl;
    exit(1);
  }

  // Attempt to login using the provided username
  if (!login(username, fd)) {
    exit(1);
  }

  // Push the value onto the stack and set the key in the table
  if (!push(value, fd)) {
    exit(1);
  }

  // Set the key in the table
  if (!set(table, key, fd)) {
    exit(1);
  }

  // Send BYE message to server
  std::string bye = "BYE\n";

  // Write BYE message to server
  rio_writen(fd, bye.c_str(), bye.size());

  // Close the connection
  close(fd);

  // Exit successfully
  return 0;
}
