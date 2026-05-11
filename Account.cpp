// ============================================================================
// Account.cpp — Abstract Base Account Class implementation
// OOP Concept: Encapsulation — private data initialised once here; sub-classes
//              access credentials only through the defined public getters.
// ============================================================================
#include "Account.h"

// OOP: Inheritance — this constructor is invoked by Creator's (and Viewer's)
//      member-initialiser list to set the shared username/password/role fields.
Account::Account(const std::string& uname,
                 const std::string& pwd,
                 const std::string& role)
    : username(uname), password(pwd), role(role) {}

// --- Getters (Encapsulation — controlled read access to private data) ---
std::string Account::getUsername() const { return username; }
std::string Account::getPassword() const { return password; }
std::string Account::getRole()     const { return role;     }
