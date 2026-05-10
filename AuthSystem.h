#ifndef AUTHSYSTEM_H
#define AUTHSYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// User class
class User {
public:
    string username;
    string password;
    string role;

    User(string u, string p, string r) {
        username = u;
        password = p;
        role = r;
    }
};

// Authentication System class
class AuthSystem {
private:
    vector<User> users;
    string fileName;

public:
    AuthSystem(string file = "users.txt");

    void loadUsers();
    void saveUsers();

    bool signUp(string username, string password, string role);
    bool login(string username, string password);

    void showMenu();
};

#endif