// ============================================================================
// VideoLibrary.h — HashMap-based Video Library
// OOP Concept: Encapsulation — Custom HashMap DSA wrapped inside its own class
// DSA: HashMap (hash table with chaining for O(1) average lookups)
// ============================================================================
#ifndef VIDEOLIBRARY_H
#define VIDEOLIBRARY_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include "Video.h"

// OOP Concept: Encapsulation — HashMap data structure wrapped in a class
// Internal hash table and nodes are private, accessed only through public methods
class VideoLibrary {
private:
    static const int TABLE_SIZE = 100;

    // Internal hash node structure (Encapsulation — hidden from outside)
    struct HashNode {
        std::string key;
        Video value;
        HashNode* next;
        HashNode(const std::string& k, const Video& v)
            : key(k), value(v), next(nullptr) {}
    };

    HashNode* table[TABLE_SIZE]; // Hash table array (private)

    // Private: Hash function (Encapsulation — hidden implementation detail)
    int hashFunction(const std::string& key) const;

public:
    // Constructor & Destructor
    VideoLibrary();
    ~VideoLibrary();

    // Insert or update a video in the library
    void insert(const std::string& key, const Video& video);

    // Find a video by title; returns true if found and sets the output parameter
    bool find(const std::string& key, Video& video) const;

    // Remove a video by title
    bool remove(const std::string& key);

    // Iterate over all videos with a callback function
    void iterate(std::function<void(const std::string&, const Video&)> callback) const;

    // Get total number of videos
    int size() const;

    // Check if the library is empty
    bool isEmpty() const;
};

#endif // VIDEOLIBRARY_H
