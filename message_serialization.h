// message_serialization.cpp

// Guards
#ifndef MESSAGE_SERIALIZATION_H
#define MESSAGE_SERIALIZATION_H

// Headers
#include "message.h"

namespace MessageSerialization {

  // Encodes a Message object into a string suitable for transmission.
  // Parameters:
  //   msg - message object to encode
  //   encoded_msg - string to store encoded message
  // Returns:
  //   void
  void encode(const Message &msg, std::string &encoded_msg);

  // Decodes a string into a Message object.
  // Parameters:
  //   encoded_msg - string containing encoded message
  //   msg - message object to store decoded message
  // Returns:
  //   void
  void decode(const std::string &encoded_msg, Message &msg);

  // Splits a string by a delimiter into a vector of strings.
  // Parameters:
  //   s - string to split
  //   delim - character used as a delimiter
  // Returns:
  //   vector of strings resulting from the split
  std::vector<std::string> split(const std::string &s, char delim);

  // Processes a vector of strings to handle quoted text properly.
  // Parameters:
  //   res - vector of strings initially split by 'split'
  // Returns:
  //   vector of strings after processing quotes
  std::vector<std::string> split_quote(std::vector<std::string> res);

  // Converts a MessageType and its associated data into a vector of strings.
  // Parameters:
  //   type - message type to convert
  //   msg - message object containing data for the type
  // Returns:
  //   vector of strings representing the type and its data
  std::vector<std::string> type_to_strings(const MessageType type, const Message &msg);

  // Converts a string to a MessageType enum.
  // Parameters:
  //   typeString - string representing the message type
  // Returns:
  //   MessageType corresponding to the string
  MessageType stringToMessageType(const std::string& typeString);
};

// End Guards
#endif // MESSAGE_SERIALIZATION_H
