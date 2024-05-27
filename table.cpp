// table.cpp

// Headers
#include <cassert>
#include "table.h"
#include "exceptions.h"
//#include "guard.h"

// Namespaces
using std::mutex;
using std::vector;
using std::map;

// Constructor
Table::Table( const std::string &name )
  : m_name( name )
{
    pthread_mutex_init(&m_mutex, nullptr);
}

// Destructor
Table::~Table()
{
    // destroy the mutex
    pthread_mutex_destroy(&m_mutex);
}

// Lock functions
// Parameters:
//   void
// Returns:
//   void
void Table::lock()
{
  // lock
//   m_mutex.lock();
  pthread_mutex_lock(&m_mutex);
}

// Unlock functions
// Parameters:
//   void
// Returns:
//   void
void Table::unlock()
{
  // unlock
//   m_mutex.unlock();
    pthread_mutex_unlock(&m_mutex); 
}

// Trylock functions
// Parameters:
//   void
// Returns:
//   bool - true if lock can be acquired, false otherwise
bool Table::trylock()
{
  // return if lock can be done
  int lock = pthread_mutex_trylock(&m_mutex);
  return lock == 0;
  
}

// Set function
// Parameters:
//   key - key to set
//   value - value to set
// Returns:
//   void
void Table::set( const std::string &key, const std::string &value )
{
  // initially set the key, value in the temporary table
  proposed_changes[key] = value;
   
}

// Get function
// Parameters:
//   key - key to get
// Returns:
//   std::string - value of the key
std::string Table::get( const std::string &key )
{
  // key can be gotten from either the temporary map or the actual table
  if (m_map.count(key) == 0 && proposed_changes.count(key) == 0) {
    throw std::invalid_argument("key not in table");
  }

  // return the value of the key
  if (proposed_changes.count(key) == 1) {
    return proposed_changes[key];
  }

  // return the value of the key
  return m_map[key];
}

// Has key function
// Parameters:
//   key - key to check
// Returns:
//   bool - true if key exists, false otherwise
bool Table::has_key( const std::string &key )
{
  // key exists in either the temporary map or the actual table
  return (m_map.count(key) == 1) || (proposed_changes.count(key) == 1);
}

// Commit changes
// Parameters:
//   void
// Returns:
//   void
void Table::commit_changes()
{
  // add data from temporary map to actual table
  for (const auto& pair : proposed_changes) {
    m_map[pair.first] = pair.second;
  }

  // clear the temporary map
  proposed_changes.clear();
}

// Rollback changes
// Parameters:
//   void
// Returns:
//   void
void Table::rollback_changes()
{
  // clear the temporary map
  proposed_changes.clear();
}

// Get mutex
// Parameters:
//   void
// Returns:
//   pthread_mutex_t* - pointer to the mutex
pthread_mutex_t* Table::get_mutex()
{
  // return the mutex
  return &m_mutex;
}