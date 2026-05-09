// ============================================================================
// WatchHistory.cpp — Stack-based Watch History implementation
// OOP Concept: Encapsulation — Internal stack logic hidden from caller
// DSA: Stack (LIFO)
// ============================================================================
#include "WatchHistory.h"
#include <stdexcept>

using std::cout;
using std::string;

// Push a video title onto the watch history stack
void WatchHistory::push(const string& title) {
    elements.push_back(title);
}

// Pop the most recent entry from watch history
void WatchHistory::pop() {
    if (!elements.empty()) {
        elements.pop_back();
    } else {
        cout << " Watch history is empty.\n";
    }
}

// Get the most recently watched video (Encapsulation — controlled access)
string WatchHistory::top() const {
    if (!elements.empty()) {
        return elements.back();
    }
    throw std::runtime_error(" Watch history is empty.");
}

// Check if watch history is empty
bool WatchHistory::isEmpty() const {
    return elements.empty();
}

// Get the number of entries
int WatchHistory::size() const {
    return static_cast<int>(elements.size());
}

// Display all watch history entries (most recent first — Stack behavior)
void WatchHistory::display() const {
    if (elements.empty()) {
        cout << " No videos watched yet.\n";
        return;
    }
    cout << " Watch History (most recent first):\n";
    for (int i = static_cast<int>(elements.size()) - 1; i >= 0; --i) {
        cout << "   " << (static_cast<int>(elements.size()) - i) << ". " << elements[i] << "\n";
    }
}

// Clear all watch history
void WatchHistory::clear() {
    elements.clear();
    cout << " Watch history cleared.\n";
}
