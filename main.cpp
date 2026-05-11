#include <iostream>
#include <string>
#include "Account.h"
#include "Video.h"
#include "VideoLibrary.h"
#include "WatchHistory.h"
#include "Playlist.h"
#include "Category.h"
#include "VideoGraph.h"
#include "AuthSystem.h"
#include "Creator.h"
#include "User.h"

using std::cout;
using std::cin;
using std::string;

int main() {
    AuthSystem authSystem("users.txt");
    VideoLibrary videoLibrary;
    CategoryTree categoryTree;
    VideoGraph videoGraph;

    cout << "\n ========================================\n";
    cout << "    Welcome to Streamify!\n";
    cout << "    Console-Based Video Streaming Platform\n";
    cout << " ========================================\n";

    //  Outer loop: returns here after logout
    while (true) {
        //  Authentication Loop 
        while (!authSystem.isAuthenticated()) {
            cout << "\n === User Authentication ===\n";
            cout << " 1. Sign Up\n";
            cout << " 2. Log In\n";
            cout << " 3. Exit\n";
            cout << " Enter your choice: ";

            int select;
            cin >> select;

            switch (select) {
                case 1:
                    authSystem.interactiveSignUp();
                    break;
                case 2:
                    if (authSystem.interactiveLogin()) {
                        cout << " Login successful! Welcome, "
                             << authSystem.getCurrentUser() << "!\n";
                    }
                    break;
                case 3:
                    cout << " Thank you for using Streamify! Goodbye!\n";
                    return 0;
                default:
                    cout << " Invalid choice. Please try again.\n";
            }
        }

        //Role-specific object based on login
        string role = authSystem.getCurrentRole();
        string uname = authSystem.getCurrentUser();

        if (role == "creator") {
            // Creator session
            Creator creator(uname, "", videoLibrary, categoryTree, videoGraph);
            int choice;
            bool running = true;

            while (running) {
                creator.displayMenu();
                cin >> choice;

                switch (choice) {
                    case 1:
                        creator.uploadVideo();
                        break;
                    case 2:
                        creator.viewVideos();
                        break;
                    case 3:
                        creator.addCategory();
                        break;
                    case 4:
                        creator.uploadVideoToCategory();
                        break;
                    case 5:
                        creator.viewCategoryVideos();
                        break;
                    case 6:
                        creator.viewVideoConnections();
                        break;
                    case 7:
                        authSystem.logout();
                        running = false;
                        break;
                    default:
                        cout << " Invalid choice. Please try again.\n";
                }
            }
        } else {
            // User session
            User user(uname, "", videoLibrary);
            int choice;
            bool running = true;

            while (running) {
                user.displayMenu();
                cin >> choice;

                switch (choice) {
                    case 1:
                        user.viewAvailableVideos();
                        break;
                    case 2:
                        user.viewWatchHistory();
                        break;
                    case 3:
                        user.addToPlaylist();
                        break;
                    case 4:
                        user.playFromPlaylist();
                        break;
                    case 5:
                        user.viewPlaylist();
                        break;
                    case 6:
                        authSystem.logout();
                        running = false;
                        break;
                    default:
                        cout << " Invalid choice. Please try again.\n";
                }
            }
        }
    } // logout returns here

    return 0;
}
