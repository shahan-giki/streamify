// ============================================================================
// Video.cpp — Video class implementation
// OOP Concept: Encapsulation — Implementation hidden from interface
// ============================================================================
#include "Video.h"

using std::cout;
using std::string;

// Default constructor
Video::Video() : title(""), filePath(""), uploader(""), likes(0) {}

// Parameterized constructor
Video::Video(const string& title, const string& filePath, const string& uploader)
    : title(title), filePath(filePath), uploader(uploader), likes(0) {}

// --- Getters (Encapsulation) ---
string Video::getTitle() const { return title; }
string Video::getFilePath() const { return filePath; }
string Video::getUploader() const { return uploader; }
int Video::getLikes() const { return likes; }
const std::vector<string>& Video::getComments() const { return comments; }

// --- Setters (Encapsulation) ---
void Video::setTitle(const string& t) { title = t; }
void Video::setFilePath(const string& fp) { filePath = fp; }
void Video::setUploader(const string& u) { uploader = u; }

// --- Business Logic ---

// OOP Concept: Encapsulation — Controlled modification of private member
void Video::addLike() {
    likes++;
}

// OOP Concept: Encapsulation — Controlled modification of private member
void Video::addComment(const string& comment) {
    comments.push_back(comment);
}

// Display all comments for this video
void Video::displayComments() const {
    if (comments.empty()) {
        cout << " No comments yet.\n";
        return;
    }
    cout << " Comments:\n";
    for (const auto& comment : comments) {
        cout << "   - " << comment << "\n";
    }
}

// Display video information
void Video::displayInfo() const {
    cout << " Title: " << title << "\n";
    cout << " File: " << filePath << "\n";
    cout << " Uploader: " << uploader << "\n";
    cout << " Likes: " << likes << "\n";
    displayComments();
}
