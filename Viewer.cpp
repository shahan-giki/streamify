// ============================================================================
// Viewer.cpp — Viewer class implementation
// When a video is "watched" it is opened in the system default media player
// (VLC, Windows Media Player, etc.) via ShellExecuteW in UI.h
// ============================================================================
#include "Viewer.h"
#include "UI.h"
#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::string;
using std::getline;

// ============================================================================
// Constructor — OOP: Inheritance
// ============================================================================
Viewer::Viewer(const string& uname, const string& pwd,
               VideoLibrary& vl, WatchHistory& wh)
    : Account(uname, pwd, "viewer"),
      videoLibrary(vl), watchHistory(wh) {}

// ============================================================================
// displayMenu — OOP: Polymorphism (override of Account::displayMenu)
// ============================================================================
void Viewer::displayMenu() {
    printHeader("  VIEWER MENU  [ " + getUsername() + " ]", Color::MAGENTA);
    printOption(1, "Browse Available Videos",    Color::CYAN,    Color::WHITE);
    printOption(2, "Watch a Video  (opens in player)", Color::GREEN,   Color::WHITE);
    printOption(3, "Like a Video",               Color::YELLOW,  Color::WHITE);
    printOption(4, "Comment on a Video",         Color::YELLOW,  Color::WHITE);
    printOption(5, "View Watch History  [Stack]",Color::BLUE,    Color::WHITE);
    printOption(6, "Add to Playlist  [Queue]",   Color::MAGENTA, Color::WHITE);
    printOption(7, "View Playlist",              Color::MAGENTA, Color::WHITE);
    printOption(8, "Play Next in Playlist",      Color::GREEN,   Color::WHITE);
    printOption(9, "Logout",                     Color::RED,     Color::WHITE);
}

// ============================================================================
// browseVideos — list all videos in the library with index numbers
// ============================================================================
void Viewer::browseVideos() const {
    printHeader("  AVAILABLE VIDEOS", Color::CYAN);

    int idx = 1;
    bool found = false;
    videoLibrary.iterate([&](const string& title, const Video& video) {
        found = true;
        cout << Color::CYAN << Color::BOLD << "  [" << idx++ << "] "
             << Color::YELLOW << title << "\n"
             << Color::GRAY   << "      Uploader : " << Color::WHITE << video.getUploader() << "\n"
             << Color::GRAY   << "      Likes    : " << Color::GREEN << video.getLikes()    << "\n"
             << Color::GRAY   << "      File     : " << Color::GRAY  << video.getFilePath() << "\n"
             << Color::RESET;
        printLine(46, "\u2500", Color::GRAY);
    });
    if (!found)
        info("No videos available yet. Ask a Creator to upload some!");
}

// ============================================================================
// watchVideo
// Builds an indexed list so the user picks by number, then opens the file
// in the system default media player via ShellExecuteW.
// DSA: Stack — pushes to WatchHistory after opening.
// ============================================================================
void Viewer::watchVideo() {
    // Collect into a vector for indexed access
    std::vector<std::pair<string, Video>> list;
    videoLibrary.iterate([&](const string& title, const Video& video) {
        list.push_back({title, video});
    });

    if (list.empty()) { err("No videos available."); return; }

    printHeader("  SELECT A VIDEO TO WATCH", Color::GREEN);
    for (size_t i = 0; i < list.size(); ++i) {
        cout << Color::GREEN << Color::BOLD << "  [" << (i + 1) << "] "
             << Color::YELLOW << list[i].first
             << Color::GRAY   << "  (" << list[i].second.getUploader() << ")"
             << Color::RESET  << "\n";
    }

    prompt("Enter number");
    int choice;
    cin >> choice;
    cout << Color::RESET;

    if (choice < 1 || choice > (int)list.size()) {
        err("Invalid selection.");
        return;
    }

    const auto& [title, video] = list[choice - 1];

    cout << "\n  " << Color::CYAN << Color::BOLD
         << "▶  Now launching: " << Color::YELLOW << title
         << Color::RESET << "\n";

    // Open in default media player (VLC, WMP, etc.)
    if (openWithDefaultApp(video.getFilePath())) {
        ok("Opened in your default media player!");
    } else {
        err("Could not open the file. Check that a media player is installed.");
        info("Path: " + video.getFilePath());
    }

    // DSA: Stack — push to watch history regardless of player result
    watchHistory.push(title);
    info("Added \"" + title + "\" to your watch history.");
}

// ============================================================================
// likeVideo
// ============================================================================
void Viewer::likeVideo() {
    std::vector<std::pair<string, Video>> list;
    videoLibrary.iterate([&](const string& t, const Video& v) {
        list.push_back({t, v});
    });
    if (list.empty()) { err("No videos available."); return; }

    printHeader("  LIKE A VIDEO", Color::YELLOW);
    for (size_t i = 0; i < list.size(); ++i)
        cout << Color::YELLOW << Color::BOLD << "  [" << (i + 1) << "] "
             << Color::WHITE << list[i].first
             << Color::GRAY  << "  (" << list[i].second.getLikes() << " likes)"
             << Color::RESET << "\n";

    prompt("Enter number");
    int choice; cin >> choice; cout << Color::RESET;
    if (choice < 1 || choice > (int)list.size()) { err("Invalid selection."); return; }

    auto& [title, video] = list[choice - 1];
    video.addLike();
    videoLibrary.insert(title, video);
    ok("Liked! \"" + title + "\" now has " + std::to_string(video.getLikes()) + " like(s).");
}

// ============================================================================
// commentOnVideo
// ============================================================================
void Viewer::commentOnVideo() {
    std::vector<std::pair<string, Video>> list;
    videoLibrary.iterate([&](const string& t, const Video& v) {
        list.push_back({t, v});
    });
    if (list.empty()) { err("No videos available."); return; }

    printHeader("  COMMENT ON A VIDEO", Color::YELLOW);
    for (size_t i = 0; i < list.size(); ++i)
        cout << Color::YELLOW << Color::BOLD << "  [" << (i + 1) << "] "
             << Color::WHITE  << list[i].first << Color::RESET << "\n";

    prompt("Enter number");
    int choice; cin >> choice; cout << Color::RESET;
    if (choice < 1 || choice > (int)list.size()) { err("Invalid selection."); return; }

    auto& [title, video] = list[choice - 1];
    string comment;
    prompt("Your comment (then Enter)");
    cin.ignore();
    getline(cin, comment);
    cout << Color::RESET;

    video.addComment(getUsername() + ": " + comment);
    videoLibrary.insert(title, video);
    ok("Comment added to \"" + title + "\".");
}

// ============================================================================
// viewWatchHistory — DSA: Stack (LIFO display)
// ============================================================================
void Viewer::viewWatchHistory() {
    printHeader("  WATCH HISTORY  [Stack — most recent first]", Color::BLUE);
    watchHistory.display();
}

// ============================================================================
// addToPlaylist — DSA: Queue (enqueue)
// ============================================================================
void Viewer::addToPlaylist() {
    std::vector<std::pair<string, Video>> list;
    videoLibrary.iterate([&](const string& t, const Video& v) {
        list.push_back({t, v});
    });
    if (list.empty()) { err("No videos available."); return; }

    printHeader("  ADD TO PLAYLIST  [Queue]", Color::MAGENTA);
    for (size_t i = 0; i < list.size(); ++i)
        cout << Color::MAGENTA << Color::BOLD << "  [" << (i + 1) << "] "
             << Color::WHITE << list[i].first << Color::RESET << "\n";

    prompt("Enter number");
    int choice; cin >> choice; cout << Color::RESET;
    if (choice < 1 || choice > (int)list.size()) { err("Invalid selection."); return; }

    const string& title = list[choice - 1].first;
    playlist.push(title);
    ok("\"" + title + "\" added to playlist.");
    info("Playlist now has " + std::to_string((int)playlist.size()) + " item(s).");
}

// ============================================================================
// viewPlaylist
// ============================================================================
void Viewer::viewPlaylist() {
    printHeader("  PLAYLIST  [Queue — play order]", Color::MAGENTA);
    if (playlist.empty()) { info("Playlist is empty."); return; }

    auto temp = playlist;
    int idx = 1;
    while (!temp.empty()) {
        cout << Color::MAGENTA << Color::BOLD << "  [" << idx++ << "] "
             << Color::WHITE << temp.front() << Color::RESET << "\n";
        temp.pop();
    }
}

// ============================================================================
// playNextInPlaylist — DSA: Queue (dequeue + Stack push)
// ============================================================================
void Viewer::playNextInPlaylist() {
    if (playlist.empty()) { err("Playlist is empty. Add videos first."); return; }

    string title = playlist.front();
    playlist.pop();   // DSA: Queue dequeue

    Video video;
    if (videoLibrary.find(title, video)) {
        cout << "\n  " << Color::GREEN << Color::BOLD
             << "▶  Playing from playlist: " << Color::YELLOW << title
             << Color::RESET << "\n";

        if (openWithDefaultApp(video.getFilePath()))
            ok("Opened in your default media player!");
        else
            err("Could not open file. Check your media player.");

        watchHistory.push(title);   // DSA: Stack push
        info("Added to watch history.");
    } else {
        err("\"" + title + "\" is no longer in the library.");
    }

    if (!playlist.empty())
        info("Next in queue: \"" + playlist.front() + "\"");
    else
        info("Playlist is now empty.");
}
