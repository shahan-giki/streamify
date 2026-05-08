// ============================================================================
// Video.h — Video class header
// OOP Concept: Encapsulation — All data members are private with getters/setters
// ============================================================================
#ifndef VIDEO_H
#define VIDEO_H

#include <iostream>
#include <string>
#include <vector>

// OOP Concept: Encapsulation — Private data members accessed through public methods
class Video {
private:
    std::string title;
    std::string filePath;
    std::string uploader;
    int likes;
    std::vector<std::string> comments;

public:
    // Constructors
    Video();
    Video(const std::string& title, const std::string& filePath, const std::string& uploader = "");

    // --- Getters (Encapsulation) ---
    std::string getTitle() const;
    std::string getFilePath() const;
    std::string getUploader() const;
    int getLikes() const;
    const std::vector<std::string>& getComments() const;

    // --- Setters (Encapsulation) ---
    void setTitle(const std::string& t);
    void setFilePath(const std::string& fp);
    void setUploader(const std::string& u);

    // --- Business Logic ---
    void addLike();
    void addComment(const std::string& comment);
    void displayComments() const;
    void displayInfo() const;
};

#endif // VIDEO_H
