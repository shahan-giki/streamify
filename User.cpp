#include "User.h"
#include <unordered_map>

using std::cout;
using std::cin;
using std::string;
using std::getline;
using std::unordered_map;

// Hidden helper function: Play video using system command
static void playVideoFile(const string& videoPath) {
    string command = "start wmplayer \"" + videoPath + "\"";
    system(command.c_str());
}

// Constructor
User::User(const string& uname, const string& pwd, VideoLibrary& vl)
    : Account(uname, pwd, "user"), videoLibrary(vl) {}

//overriding pure virtual displayMenu()
void User::displayMenu() {
    cout << "\n ====== User Menu ======\n";
    cout << " 1. View & Watch Videos\n";
    cout << " 2. View Watch History\n";
    cout << " 3. Add to Playlist\n";
    cout << " 4. Play from Playlist\n";
    cout << " 5. View Playlist\n";
    cout << " 6. Logout\n";
    cout << " Enter your choice: ";
}

// View all available videos
void User::viewAvailableVideos() {
    bool hasVideos = false;
    unordered_map<int, string> videoMap;
    int index = 1;

    videoLibrary.iterate([&](const string& title, const Video& video) {
        hasVideos = true;
        videoMap[index] = title;
        cout << "   " << index << ". " << title
             << " (Likes: " << video.getLikes() << ")\n";
        index++;
    });

    if (!hasVideos) {
        cout << " No videos available.\n";
        return;
    }

    int choice;
    cout << " Enter the number of the video to watch: ";
    cin >> choice;

    if (videoMap.find(choice) != videoMap.end()) {
        watchVideo(videoMap[choice]);
    } else {
        cout << " Invalid choice.\n";
    }
}

// Watch a specific video, with options to like and comment
void User::watchVideo(const string& title) {
    Video video;
    if (!videoLibrary.find(title, video)) {
        cout << " Video not found.\n";
        return;
    }

    cout << " Now watching: " << title << "\n";
    playVideoFile(video.getFilePath());
    watchHistory.push(title);

    // comment
    string choice;
    cout << " Would you like to comment on this video? (yes/no): ";
    cin >> choice;
    if (choice == "yes") {
        string comment;
        cout << " Enter your comment: ";
        cin.ignore();
        getline(cin, comment);
        video.addComment(comment);
        cout << " Comment added.\n";
    }

    // like
    cout << " Do you want to like this video? (yes/no): ";
    cin >> choice;
    if (choice == "yes") {
        video.addLike();
        cout << " Video liked!\n";
    }

    // Sync changes in the video library
    videoLibrary.insert(title, video);
}

// View usr history
void User::viewWatchHistory() const {
    watchHistory.display();
}

// Add a video to usr playlist
void User::addToPlaylist() {
    bool hasVideos = false;
    unordered_map<int, string> videoMap;
    int index = 1;

    videoLibrary.iterate([&](const string& title, const Video& video) {
        hasVideos = true;
        videoMap[index] = title;
        cout << "   " << index << ". " << title
             << " (Likes: " << video.getLikes() << ")\n";
        index++;
    });

    if (!hasVideos) {
        cout << " No videos available.\n";
        return;
    }

    int choice;
    cout << " Enter the number of the video to add to playlist: ";
    cin >> choice;

    if (videoMap.find(choice) != videoMap.end()) {
        playlist.enqueue(videoMap[choice]);
    } else {
        cout << " Invalid choice.\n";
    }
}

// Queue in next video
void User::playFromPlaylist() {
    if (playlist.isEmpty()) {
        cout << " Playlist is empty.\n";
        return;
    }

    string title = playlist.front();
    playlist.dequeue();

    Video video;
    if (!videoLibrary.find(title, video)) {
        cout << " Video \"" << title << "\" not found in the library.\n";
        return;
    }

    cout << " Now playing from playlist: " << title << "\n";
    playVideoFile(video.getFilePath());
}

// View playlist contents
void User::viewPlaylist() const {
    playlist.display();
}
