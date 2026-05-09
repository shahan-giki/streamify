// ============================================================================
// VideoLibrary.cpp — HashMap-based Video Library implementation
// OOP Concept: Encapsulation — Hash table logic hidden from caller
// DSA: HashMap (hash table with separate chaining)
// ============================================================================
#include "VideoLibrary.h"

using std::cout;
using std::string;

// Constructor — initialize all buckets to nullptr
VideoLibrary::VideoLibrary() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}

// Destructor — free all allocated nodes
VideoLibrary::~VideoLibrary() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* current = table[i];
        while (current) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

// Private: Hash function using std::hash (Encapsulation)
int VideoLibrary::hashFunction(const string& key) const {
    return std::hash<string>{}(key) % TABLE_SIZE;
}

// Insert or update a video in the library
void VideoLibrary::insert(const string& key, const Video& video) {
    int hashIndex = hashFunction(key);
    // Check if key already exists — update it
    HashNode* current = table[hashIndex];
    while (current) {
        if (current->key == key) {
            current->value = video;
            return;
        }
        current = current->next;
    }
    // Key not found — insert at head of chain
    HashNode* newNode = new HashNode(key, video);
    newNode->next = table[hashIndex];
    table[hashIndex] = newNode;
}

// Find a video by title (Encapsulation — returns copy via output param)
bool VideoLibrary::find(const string& key, Video& video) const {
    int hashIndex = hashFunction(key);
    HashNode* current = table[hashIndex];
    while (current) {
        if (current->key == key) {
            video = current->value;
            return true;
        }
        current = current->next;
    }
    return false;
}

// Remove a video by title
bool VideoLibrary::remove(const string& key) {
    int hashIndex = hashFunction(key);
    HashNode* current = table[hashIndex];
    HashNode* prev = nullptr;
    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                table[hashIndex] = current->next;
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

// Iterate over all videos with a callback
void VideoLibrary::iterate(std::function<void(const string&, const Video&)> callback) const {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* current = table[i];
        while (current) {
            callback(current->key, current->value);
            current = current->next;
        }
    }
}

// Get total number of videos
int VideoLibrary::size() const {
    int count = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* current = table[i];
        while (current) {
            count++;
            current = current->next;
        }
    }
    return count;
}

// Check if the library is empty
bool VideoLibrary::isEmpty() const {
    return size() == 0;
}
