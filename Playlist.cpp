#include "Playlist.h"
#include <stdexcept>

using std::cout;
using std::string;

// Add video to end of playlist
void Playlist::enqueue(const string& title) {
    elements.push_back(title);
    cout << " Added to playlist: " << title << "\n";
}

// Remove front video from playlist
void Playlist::dequeue() {
    if (!elements.empty()) {
        elements.erase(elements.begin());
    } else {
        cout << " Playlist is empty.\n";
    }
}

// Get the front video of the playlist (Encapsulation — controlled access)
string Playlist::front() const {
    if (!elements.empty()) {
        return elements.front();
    }
    throw std::runtime_error(" Playlist is empty.");
}

// Check if the playlist is empty
bool Playlist::isEmpty() const {
    return elements.empty();
}

// Get the count of content in playlist
int Playlist::size() const {
    return static_cast<int>(elements.size());
}

// Display all videos playlist
void Playlist::display() const {
    if (elements.empty()) {
        cout << " Playlist is empty.\n";
        return;
    }
    cout << " Playlist (play order):\n";
    for (int i = 0; i < static_cast<int>(elements.size()); ++i) {
        cout << "   " << (i + 1) << ". " << elements[i] << "\n";
    }
}

// Clear playlist
void Playlist::clear() {
    elements.clear();
    cout << " Playlist cleared.\n";
}
