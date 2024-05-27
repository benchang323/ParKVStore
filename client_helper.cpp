// client_helper.cpp

// Headers
#include "client_helper.h"
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "csapp.h"

// Namespaces
using std::string;
using std::ostringstream;
using std::cerr;
using std::cout;
using std::endl;

// Check for errors in server responses
// Parameters:
//   response - server response as a C-style string
//   fd - file descriptor of the server connection
// Returns:
//   true if there was an error in the response, false otherwise
bool check_error(char* response, int fd) {
    // Convert the response to a string
    string s_response(response);
    ostringstream oss;

    // Check for errors
    if (s_response.substr(0,5) == "ERROR" || s_response.substr(0,6) == "FAILED") {
        // Remove the quotes from the response
        for (const char& c : s_response) {
            if (c != '\"') {
                oss << c;
            }
        }
        // Create a string for the error message
        string error_msg = oss.str();

        // Get the error message
        int gap = error_msg.find(' ');
        cerr << "Error: " << error_msg.substr(gap + 1);

        // Close the connection
        close(fd);
        return true;
    }

    // Check for unexpected "OK" response
    if (s_response != "OK\n") {
        cerr << "Error: unexpected response, should be OK, instead got "<< s_response;
        
        // Close the connection
        close(fd);
        return true;
    }

    // Return false if no errors
    return false;
}

// Check for errors specifically for the TOP command
// Parameters:
//   response - server response as a C-style string
//   fd - file descriptor of the server connection
// Returns:
//   true if there was an error or an unexpected "OK" response, false otherwise
bool check_error_top(char *response ,int fd) {
    // Convert the response to a string
    string s_response(response);

    // Create a string for the "BYE" response
    string bye = "BYE\n";

    // Output stream for the response
    ostringstream oss;

    // Check response for errors
    if (s_response == "ERROR" || s_response == "FAILURE") {
        // Remove the quotes from the response
        for (const char& c : s_response) {
            if (c != '\"') {
                oss << c;
            }
        }

        // Get the error message
        cerr << "Error: response was invalid" << endl;

        // Close the connection
        close(fd);
        return true;
    }

    // Check for unexpected "OK" response
    if (s_response == "OK\n") {
        cerr << "Error: unexpected response OK for top" << endl;
        
        // Close the connection
        close(fd);
        return true;
    }

    // Print the value
    print_value(s_response); 
    return false;
}

// Prints the retrieved value to the console.
// Parameters:
//   response - server response containing the value
void print_value(string response) {
    // Initialize the multiplier and output stream
    int mult = 1;
    ostringstream oss;

    // Extract the value from the response
    for (int i = response.length() -1; i > -1; i--) {
        // If it is a space, break
        if (response[i] == ' ') {
            break;
        }

        // Write the value to the output stream
        oss << response[i];
    }

    // Convert the output stream to a string
    string result = oss.str();

    // Reverse the string
    std::reverse(result.begin(), result.end());

    // Print the value
    cout << result << endl;
}

// Attempts to log the user in to the server.
// Parameters:
//   username - username to login
//   fd - file descriptor of the server connection
// Returns:
//   true if login successful, else false
bool login(string username, int fd) {
    // Initialize struct for reading from the server
    rio_t rio;

    // Create the LOGIN request
    string request = "LOGIN " + username + "\n";

    // Convert the request to a C string
    const char* login_req = request.c_str();

    // Send the LOGIN command
    rio_writen(fd, login_req, strlen(login_req)); 
    
    // Initialize the rio struct for reading from the server
    rio_readinitb(&rio, fd);

    // Buffer to store the response
    char buf[1024];

    // Read the response
    ssize_t n = rio_readlineb(&rio, buf, sizeof(buf));

    // Check for errors
    return !check_error(buf, fd);
}

// Sends a GET request to the server to retrieve a value associated with a key from table.
// Parameters:
//   table - name of table
//   key - key whose value is to be 
//   fd - file descriptor of the server connection
// Returns:
//   true if request successful, else false
bool get(string table, string key, int fd) {
    // Initialize struct for reading from the server
    rio_t rio;

    // Create the GET request
    std::string request = "GET " + table + " " + key + "\n";

    // Convert the request to a C string
    const char* get_req = request.c_str();

    // Send the GET command
    rio_writen(fd, get_req, strlen(get_req));

    // Initialize the rio struct for reading from the server
    rio_readinitb(&rio, fd);

    // Buffer to store the response
    char buf[1024];

    // Read the response
    ssize_t n = rio_readlineb(&rio, buf, sizeof(buf));

    // Check for errors
    return !check_error(buf, fd);
}

// Retrieves the top value from the server's response.
// Parameters:
//   fd - file descriptor of the server connection
// Returns:
//   true if top value retrieved successfully, else false
bool top(int fd) {
    // Initialize struct for reading from the server
    rio_t rio;

    // Create the TOP request
    const char* top_req = "TOP\n";

    // Send the TOP command
    rio_writen(fd, top_req, strlen(top_req));

    // Initialize the rio struct for reading from the server
    rio_readinitb(&rio, fd);

    // Buffer to store the response
    char buf[1024];

    // Read the response
    ssize_t n = rio_readlineb(&rio, buf, sizeof(buf));

    // Check for errors
    return !check_error_top(buf, fd);
}

// Pushes a value onto the server's operand stack.
// Parameters:
//   value - value to push
//   fd - file descriptor of the server connection
// Returns:
//   true if push successful, else false
bool push(string value, int fd) {
    // Initialize struct for reading from the server
    rio_t rio;

    // Create the PUSH request
    string request = "PUSH " + value + "\n";

    // Convert the request to a C string
    const char* push_req = request.c_str();

    // Send the PUSH command
    rio_writen(fd, push_req, strlen(push_req));

    // Initialize the rio struct for reading from the server
    rio_readinitb(&rio, fd);

    // Buffer to store the response
    char buf[1024];

    // Read the response
    ssize_t n = rio_readlineb(&rio, buf, sizeof(buf));

    // Check for errors
    return !check_error(buf, fd);
}

// Sets a value in a table for a given key.
// Parameters:
//   table - name of table
//   key - key to set the value for
//   fd - file descriptor of the server connection
// Returns:
//   true if set operation was successful, else false
bool set(string table, string key, int fd) {
    // Initialize struct for reading from the server
    rio_t rio;

    // Create the SET request
    string request = "SET " + table + " " + key + "\n";

    // Convert the request to a C string
    const char* set_req = request.c_str();

    // Send the SET command
    rio_writen(fd, set_req, strlen(set_req));

    // Initialize the rio struct for reading from the server
    rio_readinitb(&rio, fd);

    // Buffer to store the response
    char buf[1024];

    // Read the response
    ssize_t n = rio_readlineb(&rio, buf, sizeof(buf));

    // Check for errors
    return !check_error(buf, fd);
}

// Begins a transaction on the server.
// Parameters:
//   fd - file descriptor of server connection
// Returns:
//   true if transaction was successfully started, else false
bool begin(int fd) {
    // Initialize struct for reading from the server
    rio_t rio;

    // Create the BEGIN request
    string begin = "BEGIN\n";

    // Convert the request to a C string
    const char* begin_req = begin.c_str();

    // Send the BEGIN command
    rio_writen(fd, begin_req, strlen(begin_req));

    // Initialize the rio struct for reading from the server
    rio_readinitb(&rio, fd);

    // Buffer to store the response
    char buf[1024];

    // Read the response
    ssize_t n = rio_readlineb(&rio, buf, sizeof(buf));

    // Check for errors
    return !check_error(buf, fd);
}

// Commits current transaction on the server.
// Parameters:
//   fd - file descriptor of the server connection
// Returns:
//   true if the transaction was successfully committed, else false
bool commit(int fd) {
    // Initialize struct for reading from the server
    rio_t rio;

    // Create the COMMIT request
    string request = "COMMIT\n";

    // Convert the request to a C string
    const char* commit_req = request.c_str();

    // Send the COMMIT command
    rio_writen(fd, commit_req, strlen(commit_req));

    // Initialize the rio struct for reading from the server
    rio_readinitb(&rio, fd);

    // Buffer to store the response
    char buf[1024];

    // Read the response
    ssize_t n = rio_readlineb(&rio, buf, sizeof(buf));

    // Check for errors
    return !check_error(buf, fd);
}

// Adds two top values from the server's operand stack.
// Parameters:
//   fd - file descriptor of the server connection
// Returns:
//   true if the add operation was successful, else false
bool add(int fd) {
    // Initialize struct for reading from the server
    rio_t rio;

    // Create the ADD request
    string request = "ADD\n";

    // Convert the request to a C string
    const char* add_req = request.c_str();

    // Send the ADD command
    rio_writen(fd, add_req, strlen(add_req));  

    // Initialize the rio struct for reading from the server  
    rio_readinitb(&rio, fd);

    // Buffer to store the response
    char buf[1024];

    // Read the response
    ssize_t n = rio_readlineb(&rio, buf, sizeof(buf)); 

    // Check for errors
    return !check_error(buf, fd); 
}
