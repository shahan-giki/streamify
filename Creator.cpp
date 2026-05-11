// ============================================================================
// Creator.cpp — Creator class implementation
// OOP Concept: Inheritance  — Creator calls Account's constructor and reuses
//              its getUsername() method without rewriting it.
// OOP Concept: Polymorphism — displayMenu() is the overridden virtual method;
//              calling it through an Account pointer resolves to this version.
// OOP Concept: Encapsulation — private method scanFolder() hides the folder-
//              scanning logic so no action method repeats that code, and
//              callers outside the class never see it.
// ============================================================================
#include "Creator.h"
#include "UI.h"
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::getline;

// ============================================================================
// Constructor — OOP: Inheritance
// ============================================================================
Creator::Creator(const string& uname, const string& pwd,
                 VideoLibrary& vl, CategoryTree& ct, VideoGraph& vg)
    : Account(uname, pwd, "creator"),
      videoLibrary(vl), categoryTree(ct), videoGraph(vg) {}

// ============================================================================
// displayMenu — OOP: Polymorphism (override)
// ============================================================================
void Creator::displayMenu() {
    printHeader("  CREATOR MENU  [ " + getUsername() + " ]", Color::CYAN);
    printOption(1, "Upload Video from Folder",      Color::GREEN,   Color::WHITE);
    printOption(2, "View Uploaded Videos",          Color::CYAN,    Color::WHITE);
    printOption(3, "Add Category  [AVL Tree]",      Color::YELLOW,  Color::WHITE);
    printOption(4, "Upload Video to Category",      Color::YELLOW,  Color::WHITE);
    printOption(5, "View Category Videos",          Color::MAGENTA, Color::WHITE);
    printOption(6, "View Video Connections [Graph]",Color::BLUE,    Color::WHITE);
    printOption(7, "Logout",                        Color::RED,     Color::WHITE);
}

// ============================================================================
// Private Helper — scanFolder
// OOP: Encapsulation — folder-scanning logic lives here once and is reused by
//      uploadVideo() and uploadVideoToCategory(). Neither method (nor any
//      outside code) needs to know how files are found and filtered.
// ============================================================================
vector<string> Creator::scanFolder(const string& folderPath) const {
    vector<string> videos;
    try {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                string ext = entry.path().extension().string();
                if (ext == ".mp4" || ext == ".mkv" ||
                    ext == ".avi" || ext == ".wmv")
                    videos.push_back(entry.path().string());
            }
        }
    } catch (const std::exception& e) {
        cout << " Failed to open folder: " << e.what() << "\n";
    }
    return videos;
}

// ============================================================================
// Creator Actions
// OOP: Each method below uses the private member references (videoLibrary,
//      categoryTree, videoGraph) — Encapsulation ensures these shared objects
//      are only modified through Creator's controlled public interface.
//      getUsername() is inherited from Account — Inheritance at work.
// ============================================================================

// Upload a video from a local folder into the shared VideoLibrary
void Creator::uploadVideo() {
    string folderPath;
    cout << " Enter folder path: ";
    cin.ignore();
    getline(cin, folderPath);

    // OOP: Encapsulation — delegate scanning to the private helper
    vector<string> videos = scanFolder(folderPath);
    if (videos.empty()) {
        cout << " No videos found in the specified folder.\n";
        return;
    }

    cout << " Videos available for upload:\n";
    for (size_t i = 0; i < videos.size(); ++i)
        cout << "   " << (i + 1) << ". "
             << fs::path(videos[i]).filename().string() << "\n";

    int choice;
    cout << " Enter the number of the video to upload: ";
    cin >> choice;

    if (choice >= 1 && choice <= static_cast<int>(videos.size())) {
        string title = fs::path(videos[choice - 1]).filename().string();
        // OOP: Inheritance — getUsername() is inherited from Account
        Video video(title, videos[choice - 1], getUsername());
        videoLibrary.insert(title, video);
        cout << " Video uploaded successfully: " << title << "\n";
    } else {
        cout << " Invalid choice.\n";
    }
}

// Display all uploaded videos; iterates over the shared VideoLibrary
void Creator::viewVideos() const {
    bool hasVideos = false;
    videoLibrary.iterate([&hasVideos](const string& title, const Video& video) {
        if (!hasVideos) {
            cout << "\n === Uploaded Videos ===\n";
            hasVideos = true;
        }
        cout << " - " << title << " (Likes: " << video.getLikes() << ")\n";
        video.displayComments();
    });

    if (!hasVideos)
        cout << " No videos uploaded yet.\n";
}

// Add a new category to the shared CategoryTree
void Creator::addCategory() {
    string category;
    cout << " Enter category name: ";
    cin.ignore();
    getline(cin, category);
    categoryTree.addCategory(category); // Delegates to CategoryTree's public interface
}

// Attach a video file from a folder to an existing category in the CategoryTree
void Creator::uploadVideoToCategory() {
    string category;
    cout << " Enter category name: ";
    cin.ignore();
    getline(cin, category);

    if (!categoryTree.categoryExists(category)) {
        cout << " Category not found: " << category << "\n";
        return;
    }

    string folderPath;
    cout << " Enter folder path: ";
    getline(cin, folderPath);

    // OOP: Encapsulation — private helper reused; no duplicated scanning logic
    vector<string> videos = scanFolder(folderPath);
    if (videos.empty()) {
        cout << " No videos found in the folder.\n";
        return;
    }

    cout << " Videos available:\n";
    for (size_t i = 0; i < videos.size(); ++i)
        cout << "   " << (i + 1) << ". "
             << fs::path(videos[i]).filename().string() << "\n";

    int choice;
    cout << " Enter the number of the video to add: ";
    cin >> choice;

    if (choice >= 1 && choice <= static_cast<int>(videos.size())) {
        categoryTree.addFilePathToCategory(category, videos[choice - 1]);
        cout << " Video added to category '" << category << "'.\n";
    } else {
        cout << " Invalid choice.\n";
    }
}

// Display videos in a chosen category via CategoryTree's public interface
void Creator::viewCategoryVideos() const {
    string category;
    cout << " Enter category name: ";
    cin.ignore();
    getline(cin, category);
    categoryTree.displayCategoryVideos(category);
}

// Build sequential connections between videos and display the graph
void Creator::viewVideoConnections() {
    string folderPath;
    cout << " Enter folder path to build connections: ";
    cin.ignore();
    getline(cin, folderPath);

    videoGraph.clear();

    try {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                string title = entry.path().stem().string();
                videoGraph.addNode(title);
            }
        }
    } catch (const std::exception& e) {
        cout << " Failed to open folder: " << e.what() << "\n";
        return;
    }

    videoGraph.buildSequentialConnections();
    videoGraph.displayGraph();
}