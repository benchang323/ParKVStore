// Guards
#ifndef SERVER_H
#define SERVER_H

// Headers
#include <map>
#include <unordered_map>
#include <string>
#include <pthread.h>
#include <memory>
#include <stack>
#include <vector>
#include "table.h"
#include "client_connection.h"

class Server {
private:
    // Mutex to protect the stack
    pthread_mutex_t mutex;
    // Variable to keep track of the client connections
    int listenfd;
    // Variable to keep track of the transaction status
    bool inTransaction = false;
    // Variable to keep track of the client connections
    std::unordered_map<std::string, Table*> tables; 
    
    // Prohibit copying and assignment
    // Copy Constructor
    Server(const Server&);
    // Assignment Operator
    Server& operator=(const Server&);

public:
    // Constructor
    Server();

    // Destructor
    ~Server();

    // This function listens for incoming connections
    // Parameters:
    //  port - port number
    // Returns:
    //  void
    void listen(const std::string &port);

    // This function starts the server loop
    // Parameters:
    //  none
    // Returns:
    //  void
    void server_loop();

    // This function creates a new client connection
    // Parameters:
    //  arg - pointer to the server object
    // Returns:
    //  void
    static void* client_worker(void* arg);

    // This function logs an error message
    // Parameters:
    //  what - error message
    // Returns:
    //  void
    void log_error(const std::string &what);

    // This function creates a new table
    // Parameters:
    //  name - table name
    // Returns:
    //  void
    void create_table(const std::string &name);

    // This function finds a table
    // Parameters:
    //  name - table name
    // Returns:
    //  Table* - pointer to the table
    Table* find_table(const std::string &name);
};

#endif // SERVER_H