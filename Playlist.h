#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <iostream>
#include <string>
#include <vector>
// All internal storage is private, accessed only through public methods
class Playlist {
private:
    std::vector<std::string> elements; // Internal queue storage

public:
    // Add a video to playlist (enqueue)
    void enqueue(const std::string& title);

    // Remove frm playlist (dequeue)
    void dequeue();

    // Get the front video of the playlist without removing it
    std::string front() const;

    // Check if the playlist is empty
    bool isEmpty() const;

    // Get the number of videos in the playlist
    int size() const;

    // Display playlisty
    void display() const;

    // Clear playlist
    void clear();
};

#endif
