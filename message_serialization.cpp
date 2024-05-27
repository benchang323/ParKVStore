// message_serialization.cpp

// Headers
#include <utility>
#include <sstream>
#include <cassert>
#include <iostream>
#include <regex>
#include <map>
#include <string>
#include "exceptions.h"
#include "message_serialization.h"
#include <unordered_map>

// Namespaces
using std::string;
using std::vector;
using std::stringstream;
using std::cout;
using std::endl;
using std::unordered_map; 

// Encodes a Message object into a string suitable for transmission.
// Parameters:
//   msg - message object to encode
//   encoded_msg - string to store encoded message
// Returns:
//   void
void MessageSerialization::encode( const Message &msg, std::string &encoded_msg) {
  // Vector to store strings for message type and arguments
  vector<string> type_and_args = type_to_strings(msg.get_message_type(), msg);

  // Stringstream to store encoded message
  stringstream ss;

  // Iterate through vector to add strings to stringstream
  for (const string &str : type_and_args) {
    ss << str;
  }
  ss << '\n';

  // Store stringstream as string
  encoded_msg = ss.str();

  // Check if encoded message is too long
  if (encoded_msg.length() > 1024) {
    throw InvalidMessage("encoded message length too long");
  }
}

// Decodes a string into a Message object.
// Parameters:
//   encoded_msg - string containing encoded message
//   msg - message object to store decoded message
// Returns:
//   void
void MessageSerialization::decode( const std::string &encoded_msg_, Message &msg) {
  // Message object to store decoded message
  Message new_message;

  // Check if encoded message is too long
  if (encoded_msg_.length() > 1024) {
    throw InvalidMessage("encoded message length too long");
  }

  // Check if encoded message is not terminated by newline character
  if (encoded_msg_[encoded_msg_.length() - 1] != '\n') {
    throw InvalidMessage("encoded message not terminated by new line character");
  }

  // Split encoded message by space
  vector<string> args = split(encoded_msg_, ' ');

  // Check if encoded message is empty
  if (args.size() == 1 && args[0][args[0].size() - 1] == '\n') { 
    args[0].pop_back();
  }

  // Convert the first element of the vector to MessageType
  MessageType m_type = stringToMessageType(args[0]);
  
  // Set message type
  new_message.set_message_type(m_type);

  // Iterate through vector to add arguments to message object
  for (size_t i = 1; i < args.size(); i++) {
    // Remove newline character if it is the last argument
    if (i == args.size() - 1 && args[i].back() == '\n') {
      args[i].pop_back();
    } 

    // Add argument to message object
    new_message.push_arg(args[i]);
  }

  // Check if message object is valid
  if (!new_message.is_valid()) {
    throw InvalidMessage("Message object isn't valid");
  }

  // Store message object in parameter
  msg = new_message;
}

// Splits a string by a delimiter into a vector of strings.
// Parameters:
//   s - string to split
//   delim - character used as a delimiter
// Returns:
//   vector of strings resulting from the split
vector<string> MessageSerialization::split(const string &s, char delim) {
  // Vector to store split strings
  vector<string> total;

  // Stringstream to split string
  stringstream ss (s);

  // Iterate through stringstream to split string
  string str;
  while (getline(ss, str, delim)) {
    total.push_back(str);
  }

  // Return vector of split strings
  vector<string> res;

  // Iterate through vector to remove empty strings
  for (const string& item : total) {
    // If string is not empty, add to vector
    if (item != "") {
      res.push_back(item);
    }
  }

  // If last element is newline, remove it
  if (res[res.size() - 1] == "\n") {
    res.pop_back();
  }

  // If first element is FAILED or ERROR, process quotes
  if (res[0] == "FAILED" || res[0] == "ERROR") {
    return split_quote(res);
  }

  // Return vector of split strings
  return res;
}

// Processes a vector of strings to handle quoted text properly.
// Parameters:
//   res - vector of strings initially split by 'split'
// Returns:
//   vector of strings after processing quotes
vector<string> MessageSerialization::split_quote(vector<string> old) {
  // Vector to store processed strings
  vector<string> res;

  // Store first element of vector
  res.push_back(old[0]);

  // Stringstream to store quoted text
  std::ostringstream oss;

  // Iterate through vector to process quoted text
  for (size_t i = 1; i < old.size(); ++i) {
    for (const char &c : old[i]) {
      // If character is a quote, add to stringstream
      if (c != '\"') {
        oss << c;
      }
    }
    // Add space after each word
    oss << " ";
  }
  // Store stringstream as string
  string quote = oss.str();

  // Remove extra space at end
  quote.pop_back(); 

  // Add processed string to vector
  res.push_back(quote);
  
  // Return processed vector
  return res;
}
  
// Converts a MessageType and its associated data into a vector of strings.
// Parameters:
//   type - message type to convert
//   msg - message object containing data for the type
// Returns:
//   vector of strings representing the type and its data
vector<string> MessageSerialization::type_to_strings(const MessageType type, const Message &msg) {
  // Switch statement to handle each MessageType
  switch (type) { 
    // Return vector of strings based on MessageType  
    case MessageType::NONE: return {"NONE"};
    case MessageType::LOGIN: return {"LOGIN ", msg.get_username()};
    case MessageType::CREATE: return {"CREATE ", msg.get_table()};
    case MessageType::PUSH: return {"PUSH ", msg.get_value()};
    case MessageType::POP: return {"POP"};
    case MessageType::TOP: return {"TOP"};
    case MessageType::SET: return {"SET ", msg.get_table(), " ", msg.get_key()};
    case MessageType::GET: return {"GET ", msg.get_table(), " ", msg.get_key()};
    case MessageType::ADD: return {"ADD"};
    case MessageType::SUB: return {"SUB"};
    case MessageType::MUL: return {"MUL"};
    case MessageType::DIV: return {"DIV"};
    case MessageType::BEGIN: return {"BEGIN"};
    case MessageType::COMMIT: return {"COMMIT"};
    case MessageType::BYE: return {"BYE"};
    case MessageType::OK: return {"OK"};
    case MessageType::FAILED: return {"FAILED ", msg.get_quoted_text()};
    case MessageType::ERROR: return {"ERROR ", msg.get_quoted_text()};
    case MessageType::DATA: return {"DATA ", msg.get_value()};
    default: return {"Unknown MessageType"};
  }
}

// Converts a string to a MessageType enum.
// Parameters:
//   typeString - string representing the message type
// Returns:
//   MessageType corresponding to the string
MessageType MessageSerialization::stringToMessageType(const string& typeString) {
  // Map of string to MessageType
  static const unordered_map<string, MessageType> stringToTypeMap = {
      {"NONE", MessageType::NONE},
      {"LOGIN", MessageType::LOGIN},
      {"CREATE", MessageType::CREATE},
      {"PUSH", MessageType::PUSH},
      {"POP", MessageType::POP},
      {"TOP", MessageType::TOP},
      {"SET", MessageType::SET},
      {"GET", MessageType::GET},
      {"ADD", MessageType::ADD},
      {"SUB", MessageType::SUB},
      {"MUL", MessageType::MUL},
      {"DIV", MessageType::DIV},
      {"BEGIN", MessageType::BEGIN},
      {"COMMIT", MessageType::COMMIT},
      {"BYE", MessageType::BYE},
      {"OK", MessageType::OK},
      {"FAILED", MessageType::FAILED},
      {"ERROR", MessageType::ERROR},
      {"DATA", MessageType::DATA}
  };

  // Iterate through map to find MessageType
  auto it = stringToTypeMap.find(typeString);

  // Return MessageType if found
  if (it != stringToTypeMap.end()) {
      return it->second;
  } else {
      throw InvalidMessage("Invalid string for MessageType");
  }
}