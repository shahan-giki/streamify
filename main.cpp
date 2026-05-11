// ============================================================================
// main.cpp — Streamify Entry Point
// Auto-loads videos from the ./videos/ folder at startup.
// Opens videos in the system default player (VLC, WMP, etc.) via ShellExecute.
// Rich colored console UI via UI.h
// ============================================================================
#include <iostream>
#include <string>
#include <limits>
#include <filesystem>

#include "UI.h"
#include "Account.h"
#include "Creator.h"
#include "Viewer.h"
#include "VideoLibrary.h"
#include "Category.h"
#include "VideoGraph.h"
#include "WatchHistory.h"

namespace fs = std::filesystem;
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

// ---------------------------------------------------------------------------
// Flush bad input
// ---------------------------------------------------------------------------
static void clearInput() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ---------------------------------------------------------------------------
// Auto-scan ./videos/ and pre-load every video file into the library
// ---------------------------------------------------------------------------
static int autoLoadVideos(VideoLibrary& lib, VideoGraph& graph,
                           const string& folder = "videos") {
    int count = 0;
    std::error_code ec;
    if (!fs::exists(folder, ec)) return 0;

    for (const auto& entry : fs::directory_iterator(folder, ec)) {
        if (!entry.is_regular_file()) continue;
        string ext = entry.path().extension().string();
        // Lowercase extension for comparison
        for (auto& c : ext) c = (char)tolower((unsigned char)c);
        if (ext != ".mp4" && ext != ".mkv" && ext != ".avi" && ext != ".wmv") continue;

        string title    = entry.path().stem().string();
        string filepath = entry.path().string();
        Video v(title, filepath, "System");
        lib.insert(title, v);
        graph.addNode(title);
        ++count;
    }
    graph.buildSequentialConnections();
    return count;
}

// ---------------------------------------------------------------------------
// Creator session
// ---------------------------------------------------------------------------
static void runCreatorSession(Creator& creator) {
    int opt = 0;
    while (opt != 7) {
        creator.displayMenu();
        prompt();
        cin >> opt;
        if (cin.fail()) { clearInput(); opt = 0; continue; }
        cout << Color::RESET;

        switch (opt) {
            case 1: creator.uploadVideo();           break;
            case 2: creator.viewVideos();            break;
            case 3: creator.addCategory();           break;
            case 4: creator.uploadVideoToCategory(); break;
            case 5: creator.viewCategoryVideos();    break;
            case 6: creator.viewVideoConnections();  break;
            case 7: ok("Logging out..."); break;
            default: err("Invalid option. Try again.");
        }
    }
}

// ---------------------------------------------------------------------------
// Viewer session
// ---------------------------------------------------------------------------
static void runViewerSession(Viewer& viewer) {
    int opt = 0;
    while (opt != 9) {
        viewer.displayMenu();
        prompt();
        cin >> opt;
        if (cin.fail()) { clearInput(); opt = 0; continue; }
        cout << Color::RESET;

        switch (opt) {
            case 1: viewer.browseVideos();        break;
            case 2: viewer.watchVideo();          break;
            case 3: viewer.likeVideo();           break;
            case 4: viewer.commentOnVideo();      break;
            case 5: viewer.viewWatchHistory();    break;
            case 6: viewer.addToPlaylist();       break;
            case 7: viewer.viewPlaylist();        break;
            case 8: viewer.playNextInPlaylist();  break;
            case 9: ok("Logging out..."); break;
            default: err("Invalid option. Try again.");
        }
    }
}

// ============================================================================
// main
// ============================================================================
int main() {
    // Enable ANSI colors on Windows terminal
    enableANSI();
    // UTF-8 output
    SetConsoleOutputCP(CP_UTF8);

    // -----------------------------------------------------------------------
    // Shared data structures
    // -----------------------------------------------------------------------
    VideoLibrary videoLibrary;
    CategoryTree categoryTree;
    VideoGraph   videoGraph;
    WatchHistory watchHistory;

    // -----------------------------------------------------------------------
    // Auto-load videos from ./videos/ folder
    // -----------------------------------------------------------------------
    printBanner();
    int loaded = autoLoadVideos(videoLibrary, videoGraph);
    if (loaded > 0)
        ok("Auto-loaded " + std::to_string(loaded) + " video(s) from ./videos/ folder.");
    else
        info("No videos found in ./videos/ — Creators can upload manually.");

    // -----------------------------------------------------------------------
    // Main menu loop
    // -----------------------------------------------------------------------
    int mainChoice = 0;
    while (mainChoice != 3) {
        printHeader("  MAIN MENU");
        printOption(1, "Login as Creator  (upload & manage videos)");
        printOption(2, "Login as Viewer   (watch, like, playlist)");
        printOption(3, "Exit");
        prompt("Your choice");
        cin >> mainChoice;
        if (cin.fail()) { clearInput(); continue; }
        cout << Color::RESET;

        if (mainChoice == 1) {
            string uname, pwd;
            printHeader("  CREATOR LOGIN", Color::YELLOW);
            prompt("Username"); cin >> uname;
            cout << Color::RESET;
            prompt("Password"); cin >> pwd;
            cout << Color::RESET;

            Creator creator(uname, pwd, videoLibrary, categoryTree, videoGraph);
            ok("Welcome, Creator " + creator.getUsername() + "!");
            runCreatorSession(creator);

        } else if (mainChoice == 2) {
            string uname, pwd;
            printHeader("  VIEWER LOGIN", Color::MAGENTA);
            prompt("Username"); cin >> uname;
            cout << Color::RESET;
            prompt("Password"); cin >> pwd;
            cout << Color::RESET;

            Viewer viewer(uname, pwd, videoLibrary, watchHistory);
            ok("Welcome, " + viewer.getUsername() + "!");
            runViewerSession(viewer);

        } else if (mainChoice == 3) {
            printBanner();
            cout << Color::YELLOW << Color::BOLD
                 << "  Thanks for using Streamify. Goodbye!\n"
                 << Color::RESET << "\n";
        } else {
            err("Invalid choice. Enter 1, 2, or 3.");
        }
    }
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
