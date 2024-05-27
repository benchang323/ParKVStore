// get_value.cpp

// Headers
#include <iostream>
#include "csapp.h"
#include "client_helper.h"
#include <string>

// Main function
int main(int argc, char **argv)
{
  // Check for correct number of arguments
  if ( argc != 6 ) {
    std::cerr << "Usage: ./get_value <hostname> <port> <username> <table> <key>\n";
    return 1;
  }

  // Extract arguments
  std::string hostname = argv[1];
  std::string port = argv[2];
  std::string username = argv[3];
  std::string table = argv[4];
  std::string key = argv[5];

  // Establish connection using hostname and port
  int fd = open_clientfd(argv[1], argv[2]);
  if (fd < 0) { 
    std::cerr << "Error: Couldn't connect to server" << std::endl;
    exit(1);
  }

  // Login using username
  if (!login(username, fd)) {
    exit(1);
  }

  // Retrieve value from table with key
  if (!get(table, key, fd)) {
    exit(1);
  }

  // Check if top is successful
  if (!top(fd)) {
    exit(1);
  }

  // Send BYE to server 
  std::string bye = "BYE\n";
  rio_writen(fd, bye.c_str(), bye.size());

  // Close connection
  close(fd);

  // Successful exit
  return 0;
}
