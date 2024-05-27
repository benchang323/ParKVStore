// Headers
#include <iostream>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "message_serialization.h"
#include "server.h"
#include "exceptions.h"
#include "client_connection.h"
#include <regex>

// Constructor
ClientConnection::ClientConnection(Server *server, int client_fd)
    // Initialize member variables
    : m_server(server), m_client_fd(client_fd) {
    rio_readinitb(&m_fdbuf, m_client_fd);
}

// Destructor
ClientConnection::~ClientConnection() {
    // Close the client file descriptor
    Close(m_client_fd); 
}

// This method initializes the client connection that allows the client to chat with the server
// Parameters:
//   none
// Returns:
//   void
void ClientConnection::chat_with_client() {
    // Initialize buffer
    char buf[MAXLINE]; 
    // Initialize first message flag
    bool firstmsg = true;

    // Loop to read messages from client
    while (true) {
        
        // Read client's message
        if (rio_readlineb(&m_fdbuf, buf, MAXLINE) <= 0) {
            // Client closed connection or error occurred
            break; 
        }

        // Decode the message
        std::string request(buf);

        // Check if the message is valid
        Message msg;

        try {
            // Decode the message
            MessageSerialization::decode(request, msg);
            
            // Check if the first message is LOGIN
            if (firstmsg && msg.get_message_type() != MessageType::LOGIN) {
                send_response("ERROR First message must be LOGIN\n");

                // Close connection if protocol is violated
                break; 
            }
            
            // Set first message flag to false
            firstmsg = false;

            // Check if the message is valid
            if (!msg.is_valid()) {
                send_response("ERROR Invalid message format\n");
                // Continue to next message
                continue;
            }

            switch (msg.get_message_type()) {
                // SET
                case MessageType::SET: {
                    // Get table, key, and value from the message
                    std::string table = msg.get_table();
                    std::string key = msg.get_key();
                    std::string value = top_value(); 

                    if (!is_valid_key(key)) {
                        send_response(Message(MessageType::ERROR, {"Invalid key"}));
                        continue;
                    }
                    
                    // Set the value in the table
                    set_value(table, key, value);

                    // Send response to client
                    send_response(Message(MessageType::OK));
                    break;
                } 
                case MessageType::POP: {
                    // Pop the value from the stack
                    pop_value();

                    // Send response to client
                    send_response(Message(MessageType::OK));

                    break;
                }
                // CREATE
                case MessageType::CREATE: {
                    // Get table from the message
                    std::string table = msg.get_table();

                    // duplicates?
                    if (m_server->find_table(table) != nullptr) {
                        // table has been named already, cannot be duplicated
                        send_response(Message(MessageType::ERROR, {"Table created"}));
                        continue;
                    }

                    // Handle invalid table name
                    if (!is_valid_table_name(table)) {
                        send_response(Message(MessageType::ERROR, {"Invalid table name"}));
                        continue;
                    }

                    try {
                        m_server->create_table(table);
                        send_response(Message(MessageType::OK));
                    } catch (const InvalidMessage& ex) {
                        send_response(Message(MessageType::ERROR, {ex.what()}));
                        continue;
                    }

                    break;
                }
                // ADD, SUB, MUL, DIV
                case MessageType::ADD:
                case MessageType::SUB:
                case MessageType::MUL:
                case MessageType::DIV: {
                    // Check if there are at least two values in the stack
                    try {
                        // Get the right and left operands
                        int right = std::stoi(pop_value());
                        int left = std::stoi(pop_value());

                        // Perform the operation
                        int result = 0;

                        // Perform the operation based on the message type
                        // ADD
                        if (msg.get_message_type() == MessageType::ADD) {
                            result = left + right;
                        } 
                        // MUL
                        else if (msg.get_message_type() == MessageType::MUL) {
                            result = left * right;
                        } 
                        // SUB
                        else if (msg.get_message_type() == MessageType::SUB) {
                            result = left - right;
                        } 
                        // DIV
                        else if (msg.get_message_type() == MessageType::DIV) {
                            if (right == 0) throw std::runtime_error("Division by zero");
                            result = left / right;
                        }
                        // Push the result to the stack
                        push_value(std::to_string(result));
                        // Send response to client
                        send_response(Message(MessageType::OK));
                        break;
                    } catch (std::invalid_argument& iae) {
                        throw OperationException("top two values are not integers");
                    }
                }
                // PUSH
                case MessageType::PUSH: {
                    // Get the value from the message
                    std::string value = msg.get_value();
                    // Push the value to the stack
                    push_value(value);
                    // Send response to client
                    send_response(Message(MessageType::OK));
                    break;
                }
                // BYE
                case MessageType::BYE: {
                    // Send response to client
                    send_response(Message(MessageType::OK));
                    // Exit the function to end this client connection
                    return; 
                }
                // TOP
                case MessageType::TOP: {
                    // Get the top value from the stack
                    std::string top_val = top_value();
                    // Send response to client
                    send_response(Message(MessageType::DATA, {top_val}));
                    // Continue to next message
                    break;
                }
                // COMMIT
                case MessageType::COMMIT: {
                    // Commit the transaction
                    commit_transaction();   
                    // Transaction is complete
                    inTransaction = false;
                    // Send response to client
                    send_response(Message(MessageType::OK));
                    break;
                }
                // LOGIN
                case MessageType::LOGIN: {
                    std::string username = msg.get_username();
                    if (!is_valid_username(username)) {
                        send_response(Message(MessageType::ERROR, {"Invalid username"}));
                        close(m_client_fd);
                        return;
                    }
                    
                    // Send response to client
                    send_response(Message(MessageType::OK));
                    break;
                }
                // BEGIN
                case MessageType::BEGIN: {
                    // Begin a transaction
                    begin_transaction();
                    // Send response to client
                    inTransaction = true;
                    // Send response to client
                    send_response(Message(MessageType::OK));
                    break;
                }
                // GET
                case MessageType::GET: {
                    // Get table from the message
                    std::string table = msg.get_table();

                    // Get the value from the table
                    std::string key = msg.get_key();

                    if (!is_valid_key(key)) {
                        send_response(Message(MessageType::ERROR, {"Invalid key"}));
                        continue;
                    }

                    // Get the value from the table
                    std::string value = get_value(table, key);

                    

                    // Push the value to the stack
                    push_value(value); 

                    // Send response to client
                    send_response(Message(MessageType::OK));
                    break;
                }
                // Default case
                default: {
                    throw InvalidMessage("Bad message");
                    break;
                }
            }
        } 
        // Catch OperationException
        catch (const OperationException& oe) {
            if (inTransaction) {
                throw FailedTransaction("invalid operation");
            } else {
                send_response(Message(MessageType::FAILED, {oe.what()}));
            }
        }
        // Catch FailedTransaction
        catch (const FailedTransaction& fte) {
            roll_back_all();
            inTransaction = false;
            send_response(Message(MessageType::FAILED, {fte.what()}));
            
        }
        // Catch InvalidMessage
        catch (const InvalidMessage& ime) {
            send_response(Message(MessageType::ERROR, {ime.what()}));
            break;
        }
        // Catch std::exception
        catch (const std::exception& e) {
            send_response(Message(MessageType::ERROR, {e.what()}));
        }
    }
}

// This method sends a response to the client
// Parameters:
//   message - message to send
// Returns:
//   void
void ClientConnection::send_response(const Message& msg) {
    // Encode the message
    std::string response;
    MessageSerialization::encode(msg, response);

    // Response length
    size_t result = rio_writen(m_client_fd, response.c_str(), response.length());

    // Check if all bytes were written
    if (result != response.length()) {
        // Print error message
        std::cerr << "Failed to write all bytes to socket. Expected " << response.length() << ", but wrote " << result << std::endl;
        
        // Close the connection and clean up resources
        Close(m_client_fd); 

        // Set the client file descriptor to -1
        m_client_fd = -1;
        
        // Throw an exception
        throw std::runtime_error("Socket write failure, connection closed.");
    }
}

// This function pushes a value to the stack
// Parameters:
//  table - pointer to the table
// Returns:
//  void
void ClientConnection::push_value(const std::string& value) {
    // Push the value to the stack
    value_stack.push(value);
}

// This function pops a value from the stack
// Parameters:
//  none
// Returns:
//  std::string - value
std::string ClientConnection::pop_value() {
    // Check if the stack is empty
    if (value_stack.empty()) {
        throw OperationException("Stack is empty");
    }

    // Pop the value from the stack
    std::string value = value_stack.top();

    // Remove the value from the stack
    value_stack.pop();

    // Return the value
    return value;
}

// This function returns the top value from the stack
// Parameters:
//  none
// Returns:
//  std::string - value
std::string ClientConnection::top_value() {
    // Check if the stack is empty
    if (value_stack.empty()) {
        throw OperationException("Stack is empty");
    }

    // Return the top value from the stack
    std::string value = value_stack.top();

    // Return the value
    return value;
}

// This function begins a transaction
// Parameters:
//  none
// Returns:
//  void
void ClientConnection::begin_transaction() {
    // Check if a transaction is already in progress
    if (inTransaction) {
        throw std::runtime_error("Transaction already in progress");
    }

    // Set the transaction flag to true
    inTransaction = true;
}

// This function commits a transaction
// Parameters:
//  none
// Returns:
//  void
void ClientConnection::commit_transaction() {
    // Check if a transaction is in progress
    if (!inTransaction) {
        throw std::runtime_error("No transaction in progress");
    }

    // Commit the transaction
    for (auto& pair : lockedTables) {
        // Commit the changes
        pair.first->commit_changes();
        // Unlock the table
        pair.first->unlock(); 
    }

    // Clear the locked tables
    lockedTables.clear();

    // Transaction is complete
    inTransaction = false;
}

// This method rolls back all the changes made during a transaction
// Parameters:
//   none
// Returns:
//   void
void ClientConnection::roll_back_all() {
    // Roll back all the changes made during a transaction
    for (auto& pair : lockedTables) {
        // Roll back the changes
        pair.first->rollback_changes();
        // Unlock the table
        pair.first->unlock();
    }

    // Clear the locked tables
    lockedTables.clear();
}

// This function sets a value in the table
// Parameters:
//  table - table name
//  key - key
//  value - value
// Returns:
//  void
void ClientConnection::set_value(const std::string& table, const std::string& key, const std::string& value) {
    // Find the table
    Table* t = m_server->find_table(table);
    
    // Check if the table exists
    if (t) {
        // Set the value in the table
        if (inTransaction) {
            // Check if the table is already locked
            if (lockedTables.count(t) == 0 && !t->trylock()) {
            // Error if the table is already locked
                throw FailedTransaction("attempted to acquire a lock already in use");
            }

            // Lock the table
            lockedTables[t] = true;

            // Set the value in the table
            t->set(key, value);
        } else {
            // Lock the table
            t->lock();
            // Set the value in the table
            t->set(key, value);
            // Commit the changes
            t->commit_changes();
            // Unlock the table
            t->unlock();
        }
    } else {
        // Error if the table is not found
        throw std::runtime_error("Table not found");
    }
}

// This function gets a value from the table
// Parameters:
//  table - table name
//  key - key
// Returns:
//  std::string - value
std::string ClientConnection::get_value(const std::string& table, const std::string& key) {
    // Find the table
    Table* t = m_server->find_table(table);
    
    // Check if the table exists
    std::string value;

    // Get the value from the table
    if (!t) {
        throw std::runtime_error("Table not found");
    }

    // If in transaction
    if (inTransaction) {
        // Check if the table is already locked
        if (lockedTables.count(t) == 0 && !t->trylock()) {
            throw FailedTransaction("attempted to acquire a lock already in use");
        }

        // Lock the table
        lockedTables[t] = true;

        // Get the value from the table
        value = t->get(key);
    } else {
        // Lock the table
        t->lock();

        // Get the value from the table
        value = t->get(key);

        // Unlock the table
        t->commit_changes();

        // Unlock the table
        t->unlock();
    }

    // Return the value
    return value;
}

bool is_valid_username(const std::string& username) {
    // Check if username follows the identifier rules
    std::regex pattern("^[a-zA-Z][a-zA-Z0-9_]*$");
    return std::regex_match(username, pattern);
}

bool is_valid_table_name(const std::string& name) {
    // Check if table name follows the identifier rules
    std::regex pattern("^[a-zA-Z][a-zA-Z0-9_]*$");
    return std::regex_match(name, pattern);
}

bool is_valid_key(const std::string& key) {
    // Check if key follows the identifier rules
    std::regex pattern("^[a-zA-Z][a-zA-Z0-9_]*$");
    return std::regex_match(key, pattern);
}