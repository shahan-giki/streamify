#include "AuthSystem.h"

// Constructor
AuthSystem::AuthSystem(string file) {
    fileName = file;
    loadUsers();
}

// Load users from file
void AuthSystem::loadUsers() {
    ifstream fin(fileName);

    string u, p, r;

    while (fin >> u >> p >> r) {
        users.push_back(User(u, p, r));
    }

    fin.close();
}

// Save users to file
void AuthSystem::saveUsers() {
    ofstream fout(fileName);

    for (int i = 0; i < users.size(); i++) {
        fout << users[i].username << " "
             << users[i].password << " "
             << users[i].role << endl;
    }

    fout.close();
}

// Sign Up
bool AuthSystem::signUp(string username, string password, string role) {

    // Check if username already exists
    for (int i = 0; i < users.size(); i++) {
        if (users[i].username == username) {
            cout << "Username already exists!\n";
            return false;
        }
    }

    // Add new user
    users.push_back(User(username, password, role));

    saveUsers();

    cout << "Account created successfully!\n";
    return true;
}

// Login
bool AuthSystem::login(string username, string password) {

    for (int i = 0; i < users.size(); i++) {

        if (users[i].username == username &&
            users[i].password == password) {

            cout << "Login Successful!\n";
            cout << "Role: " << users[i].role << endl;

            return true;
        }
    }

    cout << "Invalid username or password!\n";
    return false;
}

// Menu
void AuthSystem::showMenu() {

    int choice;

    do {
        cout << "\n===== AUTH SYSTEM =====\n";
        cout << "1. Sign Up\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {

            string u, p, r;

            cout << "Enter Username: ";
            cin >> u;

            cout << "Enter Password: ";
            cin >> p;

            cout << "Enter Role (creator/user): ";
            cin >> r;

            signUp(u, p, r);
        }

        else if (choice == 2) {

            string u, p;

            cout << "Enter Username: ";
            cin >> u;

            cout << "Enter Password: ";
            cin >> p;

            login(u, p);
        }

    } while (choice != 3);
}