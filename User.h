#ifndef USER_H
#define USER_H

#include "Account.h"
#include "VideoLibrary.h"
#include "WatchHistory.h"
#include "Playlist.h"

//  Inheritance IS-A Account Polymorphism Overrides displayMenu() from Account
class User : public Account {
private:
    VideoLibrary& videoLibrary;  // Reference to video library
    WatchHistory watchHistory;   // Stacking history (Composition)
    Playlist playlist;           // queue (Composition)

public:
    // Constructor
    User(const std::string& uname, const std::string& pwd, VideoLibrary& vl);

    // Override of pure virtual function
    void displayMenu() override;

    // User-specific methods
    void viewAvailableVideos();
    void watchVideo(const std::string& title);
    void viewWatchHistory() const;
    void addToPlaylist();
    void playFromPlaylist();
    void viewPlaylist() const;
};

#endif 
