// ============================================================================
// Creator.h — Creator class header
// OOP Concept: Inheritance  — Creator IS-A Account; it extends the base class
//              with creator-specific data and behaviour.
// OOP Concept: Polymorphism — displayMenu() overrides Account's pure virtual,
//              so calling displayMenu() on an Account* at runtime invokes the
//              correct Creator version (runtime polymorphism / vtable dispatch).
// OOP Concept: Encapsulation — private data members are accessible only
//              through the defined public methods.
// ============================================================================
#ifndef CREATOR_H
#define CREATOR_H

#include "Account.h"
#include "VideoLibrary.h"
#include "Category.h"
#include "VideoGraph.h"

// OOP: Inheritance — Creator publicly inherits Account.
//      It gains username/password management and the role field for free,
//      and only adds what is specific to a creator account.
class Creator : public Account {
private:
    // -------------------------------------------------------------------------
    // OOP: Encapsulation — these references are private.
    // Creator accesses the shared data structures only through its own methods,
    // preventing uncontrolled external access.
    // Stored as references (not copies) so all changes are immediately
    // reflected across the whole application (single source of truth).
    // -------------------------------------------------------------------------
    VideoLibrary& videoLibrary;  // Shared library of all uploaded videos
    CategoryTree& categoryTree;  // Shared AVL tree of categories
    VideoGraph&   videoGraph;    // Shared graph of video relationships

public:
    // OOP: Inheritance — constructor explicitly calls Account's constructor
    //      to initialise the inherited username, password, and role fields.
    Creator(const std::string& uname, const std::string& pwd,
            VideoLibrary& vl, CategoryTree& ct, VideoGraph& vg);

    // -------------------------------------------------------------------------
    // OOP: Polymorphism — 'override' keyword enforces that this method matches
    //      the pure virtual signature in Account exactly.
    //      At runtime, calling displayMenu() through an Account pointer on a
    //      Creator object dispatches here — not to Account's version.
    // -------------------------------------------------------------------------
    void displayMenu() override;

    // --- Creator-specific behaviour (not part of Account's interface) ---
    void uploadVideo();               // Scan a folder and upload a chosen video
    void viewVideos()          const; // List all uploaded videos with likes/comments
    void addCategory();               // Add a new category to the AVL tree
    void uploadVideoToCategory();     // Attach a video file to an existing category
    void viewCategoryVideos()  const; // Display videos belonging to a category
    void viewVideoConnections();      // Build and display the video relationship graph
};

#endif // CREATOR_H