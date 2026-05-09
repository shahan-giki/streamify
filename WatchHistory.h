// ============================================================================
// WatchHistory.h — Stack-based Watch History
// OOP Concept: Encapsulation — Stack DSA wrapped inside its own class
// DSA: Stack (LIFO — Last watched is first to display)
// ============================================================================
#ifndef WATCHHISTORY_H
#define WATCHHISTORY_H

#include <iostream>
#include <string>
#include <vector>

class WatchHistory {
private:
    std::vector<std::string> elements; // Internal stack storage

public:
    // Push a video title onto the watch history stack
    void push(const std::string& title);

    // Pop the most recent entry from watch history
    void pop();

    // Get the most recently watched video
    std::string top() const;

    // Check if watch history is empty
    bool isEmpty() const;

    // Get the number of entries in watch history
    int size() const;

    // Display all watch history entries (most recent first)
    void display() const;

    // Clear all watch history
    void clear();
};

#endif // WATCHHISTORY_H
