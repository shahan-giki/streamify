// ============================================================================
// Viewer.h — Viewer class header
// OOP Concept: Inheritance   — Viewer IS-A Account; it extends the base class
//              with viewer-specific behaviour (watching, liking, commenting,
//              watch-history management, playlist queue).
// OOP Concept: Polymorphism  — displayMenu() overrides Account's pure virtual,
//              so calling displayMenu() on an Account* that points to a Viewer
//              dispatches here at runtime (vtable dispatch).
// OOP Concept: Encapsulation — all shared data structures are held as private
//              references; accessed only through Viewer's public methods.
// ============================================================================
#ifndef VIEWER_H
#define VIEWER_H

#include "Account.h"
#include "VideoLibrary.h"
#include "WatchHistory.h"
#include <queue>
#include <string>
#include <vector>
#include <utility>

// OOP: Inheritance — Viewer publicly inherits Account.
//      Gains username/password/role management from the base class and adds
//      viewer-specific behaviour on top.
class Viewer : public Account {
private:
    // -------------------------------------------------------------------------
    // OOP: Encapsulation — shared data structures are private references.
    //      Viewer can only interact with the library through its own methods,
    //      preventing uncontrolled external access.
    // -------------------------------------------------------------------------
    VideoLibrary& videoLibrary;   // Shared library of all uploaded videos
    WatchHistory& watchHistory;   // Stack-based watch history (DSA: Stack)

    // DSA: Queue — playlist maintained as a FIFO queue of video titles
    std::queue<std::string> playlist;

public:
    // OOP: Inheritance — constructor explicitly calls Account's constructor
    //      to initialise the inherited username, password, and role fields.
    Viewer(const std::string& uname, const std::string& pwd,
           VideoLibrary& vl, WatchHistory& wh);

    // -------------------------------------------------------------------------
    // OOP: Polymorphism — 'override' enforces that this matches Account's pure
    //      virtual signature exactly. Runtime dispatch through Account* calls
    //      THIS function when the object is actually a Viewer.
    // -------------------------------------------------------------------------
    void displayMenu() override;

    // --- Viewer-specific behaviour (not part of Account's interface) ---
    void watchVideo();          // Watch a video, push to watch history (Stack)
    void likeVideo();           // Like a chosen video
    void commentOnVideo();      // Add a comment to a chosen video
    void viewWatchHistory();    // Display Stack contents (most recent first)
    void addToPlaylist();       // Enqueue a video title (Queue: FIFO)
    void viewPlaylist();        // Display current playlist queue
    void playNextInPlaylist();  // Dequeue and "play" next video (Queue)
    void browseVideos() const;  // List all available videos
};

#endif // VIEWER_H
