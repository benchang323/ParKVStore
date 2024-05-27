// table.h

// Guards
#ifndef TABLE_H
#define TABLE_H

// Includes
#include <map>
#include <string>
#include <pthread.h>
#include <mutex>
#include <vector>

class Table {
private:
  // Member variables
  // Name of the table
  std::string m_name;

  // Mutex for thread safety
  pthread_mutex_t m_mutex;

  // Map of key-value pairs
  std::map<std::string, std::string> m_map;
  
  // Map of proposed changes
  std::map<std::string, std::string> proposed_changes;

  // Copy constructor
  Table( const Table & );

  // Assignment operator
  Table &operator=( const Table & );

public:
  // Constructor
  Table( const std::string &name );

  // Destructor
  ~Table();

  // Get name
  // Parameters:
  //   void
  // Returns:
  //   std::string - name of the table
  std::string get_name() const { return m_name; }

  // Lock functions
  // Parameters:
  //   void
  // Returns:
  //   void
  void lock();

  // Unlock functions
  // Parameters:
  //   void
  // Returns:
  //   void
  void unlock();

  // Trylock functions
  // Parameters:
  //   void
  // Returns:
  //   bool - true if lock can be acquired, false otherwise
  bool trylock();

  // Note: these functions should only be called while the
  // table's lock is held!

  // Set function
  // Parameters:
  //   key - key to set
  //   value - value to set
  // Returns:
  //   void
  void set( const std::string &key, const std::string &value );

  // Has key function
  // Parameters:
  //   key - key to check
  // Returns:
  //   bool - true if key exists, false otherwise
  bool has_key( const std::string &key );

  // Get function
  // Parameters:
  //   key - key to get
  // Returns:
  //   std::string - value of the key
  std::string get( const std::string &key );

  // Commit changes
  // Parameters:
  //   void
  // Returns:
  //   void
  void commit_changes();

  // Rollback changes
  // Parameters:
  //   void
  // Returns:
  //   void
  void rollback_changes();

  // Get mutex
  // Parameters:
  //   void
  // Returns:
  //   pthread_mutex_t* - pointer to the mutex
  pthread_mutex_t* get_mutex();
};

// End of guards
#endif // TABLE_H
