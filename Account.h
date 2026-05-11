// ============================================================================
// Account.h — Abstract Base Account Class
// OOP Concept: Abstraction    — pure virtual displayMenu() defines a contract
//                               that every concrete account type MUST fulfill.
// OOP Concept: Encapsulation  — username / password / role are private; only
//                               accessible through the defined getters.
// OOP Concept: Inheritance    — Creator (and any future Viewer) publicly
//                               inherits this class, gaining auth data for free.
// OOP Concept: Polymorphism   — Account* can point to Creator or Viewer at
//                               runtime; displayMenu() dispatches correctly via
//                               the vtable (runtime polymorphism).
// ============================================================================
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

// OOP: Abstraction — Account is an abstract class (cannot be instantiated).
//      It defines the common interface for all account types.
class Account {
private:
    // OOP: Encapsulation — credentials are private; no external code can read
    //      or modify them except through the controlled getters below.
    std::string username;
    std::string password;
    std::string role;      // "creator" | "viewer"

public:
    // OOP: Inheritance — sub-classes call this constructor via their
    //      member-initialiser lists to populate the inherited fields.
    Account(const std::string& uname,
            const std::string& pwd,
            const std::string& role);

    // Virtual destructor ensures the correct derived destructor runs when
    // deleting through an Account pointer (prevents resource leaks).
    virtual ~Account() = default;

    // --- Getters (Encapsulation) ---
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getRole()     const;

    // -------------------------------------------------------------------------
    // OOP: Abstraction / Polymorphism
    // Pure virtual — Account itself has no menu. Every concrete subclass MUST
    // provide its own version. Calling through an Account* at runtime invokes
    // the correct override (vtable dispatch).
    // -------------------------------------------------------------------------
    virtual void displayMenu() = 0;
};

#endif // ACCOUNT_H
