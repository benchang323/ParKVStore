// Headers
#include <iostream>
#include "csapp.h"
#include "server.h"
#include "exceptions.h"
#include "guard.h"
#include "table.h"
#include <regex>

// Constructor
Server::Server() : listenfd(-1) {
    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);
}

// Destructor
Server::~Server() {
    // Delete all the tables
    for (auto& pair : tables) {
        delete pair.second;
    }

    // Close the listening socket
    if (listenfd != -1) {
        Close(listenfd);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);
}

// This function listens for incoming connections
// Parameters:
//  port - port number
// Returns:
//  void
void Server::listen(const std::string &port) {
    // Open the listening socket
    listenfd = Open_listenfd(port.c_str());

    // Check if the listening socket was opened successfully
    if (listenfd < 0) {
        log_error("Failed to open listening socket on port " + port);
        exit(1);
    }
}

// This function starts the server loop
// Parameters:
//  none
// Returns:
//  void
void Server::server_loop() {
    // Listen for incoming connections and create a new client connection
    while (true) {
        // Accept a new connection
        socklen_t clientlen = sizeof(struct sockaddr_storage);

        // Create a new client connection
        struct sockaddr_storage clientaddr;

        // Accept a new connection
        int connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        // Check if the connection was accepted successfully
        if (connfd < 0) {
            log_error("Failed to accept connection");
            continue;
        }

        // Create a new client connection
        ClientConnection* client = new ClientConnection(this, connfd);

        // Create a new thread for the client
        pthread_t thr_id;

        // Check if the thread was created successfully
        if (pthread_create(&thr_id, nullptr, client_worker, client) != 0) {
            log_error("Could not create client thread");
            delete client;
        }
    }
}

// This function creates a new client connection
// Parameters:
//  arg - pointer to the server object
// Returns:
//  void
void* Server::client_worker(void* arg) {
    // Detach the thread
    pthread_detach(pthread_self());

    // Cast the argument to a ClientConnection pointer
    std::unique_ptr<ClientConnection> client(static_cast<ClientConnection*>(arg));

    // Chat with the client
    client->chat_with_client();

    // Return nullptr
    return nullptr;
}

// This function logs an error message
// Parameters:
//  what - error message
// Returns:
//  void
void Server::log_error(const std::string &what) {
    // Print the error message to the standard error stream
    std::cerr << what << std::endl;
}

// This function creates a new table
// Parameters:
//  name - table name
// Returns:
//  void
void Server::create_table(const std::string &name) {
    // Check if the table name is valid
    if (!std::regex_match(name, std::regex("^[A-Za-z0-9_]+$"))) {
        throw InvalidMessage("Invalid table name");
    }

    // Lock the mutex
    if (tables.find(name) == tables.end()) {
        // Create a new table if it does not exist
        tables[name] = new Table(name);
    } else {
        // Throw an exception if the table already exists
        throw InvalidMessage("table already exists");
    }
}

// This function finds a table
// Parameters:
//  name - table name
// Returns:
//  Table* - pointer to the table
Table* Server::find_table(const std::string &name) {
    // Find the table, if it exists in the map, otherwise return nullptr
    Table* table = tables.count(name) ? tables[name] : nullptr;
    
    // Return the table
    return table;
}