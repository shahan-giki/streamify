# Streamify — Project Documentation

> **Course**: Object-Oriented Programming (CS-112)  
> **Institution**: Ghulam Ishaq Khan Institute of Engineering Sciences & Technology (GIKI)  
> **Type**: Semester Project (OOP + DSA Integration)  
> **Language**: C++17  
> **Platform**: Windows (Console Application)

---

## Table of Contents

1. [Project Overview](#1-project-overview)
2. [System Architecture](#2-system-architecture)
3. [OOP Concepts — Detailed Breakdown](#3-oop-concepts--detailed-breakdown)
4. [DSA Concepts — Detailed Breakdown](#4-dsa-concepts--detailed-breakdown)
5. [Class Reference](#5-class-reference)
6. [File Structure](#6-file-structure)
7. [UI System](#7-ui-system)
8. [Video Playback Integration](#8-video-playback-integration)
9. [How to Build & Run](#9-how-to-build--run)
10. [Feature Walkthrough](#10-feature-walkthrough)
11. [Known Limitations](#11-known-limitations)

---

## 1. Project Overview

**Streamify** is a C++17 console-based video streaming platform that simulates the core functionality of a real streaming service. The project is designed to demonstrate a deep integration of **Object-Oriented Programming** principles and **Data Structure & Algorithm** concepts in a single cohesive application.

### What It Does

- Two account roles: **Creator** (uploads, manages) and **Viewer** (watches, likes, comments)
- Auto-detects and loads video files from a local `./videos/` folder at startup
- Opens video files in the system's default media player (VLC, Windows Media Player, etc.) when a viewer selects "Watch"
- Full rich-color terminal UI using ANSI escape codes and UTF-8 box-drawing characters
- Manages video metadata, categories, watch history, playlists, and video relationship graphs entirely in-memory using custom DSA implementations

### Design Philosophy

> Every data structure is wrapped in its own class (Encapsulation).  
> Every class that represents an account type inherits from a common base (Inheritance).  
> The correct behavior is dispatched at runtime through virtual functions (Polymorphism).  
> No account type can be instantiated from the base — only concrete subtypes (Abstraction).

---

## 2. System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                         main.cpp                            │
│  - Enables ANSI/UTF-8 console                               │
│  - Auto-loads ./videos/ → VideoLibrary + VideoGraph         │
│  - Main menu loop (Creator / Viewer)                        │
└────────┬──────────────────────────────┬────────────────────┘
         │                              │
         ▼                              ▼
   ┌───────────┐                 ┌───────────┐
   │  Creator  │                 │   Viewer  │
   │ (inherits)│                 │ (inherits)│
   └─────┬─────┘                 └─────┬─────┘
         │                              │
         └──────────┬───────────────────┘
                    ▼
            ┌──────────────┐
            │   Account    │  ← Abstract Base Class
            │  (username,  │
            │   password,  │
            │   role,      │
            │   displayMenu│ ← pure virtual
            └──────────────┘

Shared Data Structures (single source of truth):
┌─────────────────┐  ┌─────────────────┐  ┌──────────────┐  ┌──────────────┐
│  VideoLibrary   │  │  CategoryTree   │  │  VideoGraph  │  │ WatchHistory │
│  (HashMap)      │  │  (AVL Tree)     │  │  (Graph)     │  │  (Stack)     │
│  O(1) avg       │  │  O(log n)       │  │  Edge-list   │  │  LIFO        │
└─────────────────┘  └─────────────────┘  └──────────────┘  └──────────────┘

Viewer-owned:
┌──────────────────┐
│  playlist        │
│  (std::queue)    │
│  FIFO            │
└──────────────────┘
```

---

## 3. OOP Concepts — Detailed Breakdown

### 3.1 Encapsulation

Encapsulation is the bundling of data with the methods that operate on it, while hiding internal implementation details.

#### `Video` class
```cpp
class Video {
private:
    std::string title;       // Never directly accessible
    std::string filePath;
    std::string uploader;
    int likes;
    std::vector<std::string> comments;
public:
    void addLike();          // Controlled modification
    void addComment(...);    // Controlled modification
    std::string getTitle(); // Controlled read access
    // ...
};
```
- `likes` can only go up via `addLike()` — no external code can set it to an arbitrary value.
- `comments` is appended to via `addComment()` — the vector itself is never exposed.

#### `VideoLibrary` class
- Internally a hash table of 100 buckets with `HashNode*` chain pointers.
- The `HashNode` struct, the `table[]` array, and the `hashFunction()` are all **private**.
- External code only sees: `insert()`, `find()`, `remove()`, `iterate()`, `size()`, `isEmpty()`.

#### `CategoryTree` class
- The `AVLNode` struct (with `left`, `right`, `height`, `filePaths`) is a **private nested struct**.
- All rotation methods (`rotateLeft`, `rotateRight`), height helpers, and recursive insert/find are **private**.
- Public interface has just 6 methods.

#### `WatchHistory` class
- The `std::vector<string> elements` that acts as the stack is **private**.
- Only push/pop/top/display are accessible — callers cannot index or iterate the vector directly.

---

### 3.2 Inheritance

```
Account (abstract)
    │
    ├── Creator  ──► adds: uploadVideo, addCategory, viewVideoConnections, ...
    │                      references: VideoLibrary&, CategoryTree&, VideoGraph&
    │
    └── Viewer   ──► adds: watchVideo, likeVideo, commentOnVideo, playlist mgmt, ...
                           references: VideoLibrary&, WatchHistory&
                           owns:       std::queue<string> playlist
```

**How it works in code:**

```cpp
// Creator.cpp — member initialiser list calls Account's constructor
Creator::Creator(const string& uname, const string& pwd,
                 VideoLibrary& vl, CategoryTree& ct, VideoGraph& vg)
    : Account(uname, pwd, "creator"),   // ← Inheritance: base ctor call
      videoLibrary(vl), categoryTree(ct), videoGraph(vg) {}
```

```cpp
// Creator can call getUsername() without defining it — inherited from Account
void Creator::uploadVideo() {
    Video video(title, filepath, getUsername());  // ← inherited method
    //...
}
```

---

### 3.3 Polymorphism

`Account` declares one **pure virtual** function:

```cpp
class Account {
public:
    virtual void displayMenu() = 0;  // Pure virtual — subclasses MUST override
};
```

Both `Creator` and `Viewer` override it with the `override` keyword:

```cpp
// Creator.h
void displayMenu() override;

// Viewer.h
void displayMenu() override;
```

At runtime, through a base-class pointer, the **correct version is dispatched via the vtable**:

```cpp
Account* account = new Creator(...);
account->displayMenu();   // → Creator::displayMenu() — at runtime!

account = new Viewer(...);
account->displayMenu();   // → Viewer::displayMenu() — at runtime!
```

This is **runtime polymorphism** — the decision of which function to call is made at execution time, not compile time.

---

### 3.4 Abstraction

`Account` cannot be instantiated:
```cpp
Account a("user", "pass", "role");  // ERROR: cannot instantiate abstract class
```

It defines *what* all accounts must be able to do (the contract — `displayMenu()`), without dictating *how*. This separates interface from implementation and forces every concrete account type to provide its own behavior.

---

## 4. DSA Concepts — Detailed Breakdown

### 4.1 HashMap — `VideoLibrary`

**Purpose**: Store and retrieve `Video` objects by title in O(1) average time.

**Implementation**: Custom hash table with **separate chaining** (each bucket is a singly-linked list of `HashNode`).

```
Table (100 buckets):
  [0]  → nullptr
  [1]  → HashNode("Tekken 7 ...") → nullptr
  [2]  → nullptr
  ...
  [47] → HashNode("EA SPORTS FC 26 ...") → HashNode("EA SPORTS FC ...") → nullptr
  ...
```

**Hash function**:
```cpp
int hashFunction(const string& key) const {
    return std::hash<string>{}(key) % TABLE_SIZE;
}
```

**Collision handling**: Separate chaining — new node inserted at head of bucket list.

**Complexity**:
| Operation | Average | Worst |
|-----------|---------|-------|
| Insert    | O(1)    | O(n)  |
| Find      | O(1)    | O(n)  |
| Remove    | O(1)    | O(n)  |
| Iterate   | O(n)    | O(n)  |

---

### 4.2 AVL Tree — `CategoryTree`

**Purpose**: Store video categories in sorted order with guaranteed O(log n) operations.

**Implementation**: Self-balancing Binary Search Tree. Each `AVLNode` stores a category name, a list of associated video file paths, and a height value.

**Balancing Cases**:
```
Left-Left   → rotateRight(node)
Right-Right → rotateLeft(node)
Left-Right  → rotateLeft(node->left), rotateRight(node)
Right-Left  → rotateRight(node->right), rotateLeft(node)
```

**Example — inserting "Sports", "Action", "Music", "Comedy"**:
```
After all inserts, tree stays balanced:
        Music
       /     \
  Action    Sports
  /
Comedy
```
*(In-order traversal gives alphabetical order: Action, Comedy, Music, Sports)*

**Complexity**:
| Operation      | Complexity |
|----------------|------------|
| Insert         | O(log n)   |
| Search         | O(log n)   |
| In-order print | O(n)       |

---

### 4.3 Graph — `VideoGraph`

**Purpose**: Model relationships between videos (e.g., "watch next" chains).

**Implementation**: Edge-list representation — two separate `std::vector`s: one for node labels (video titles) and one for `(from, to)` index pairs.

```
Nodes: ["Tekken 7 ...", "EA SPORTS FC 26 (Jan)", "EA SPORTS FC 26 (Jan 24)"]
Edges: [(0→1), (1→2)]

Displayed as:
  Tekken 7 ... → EA SPORTS FC 26 (Jan) → EA SPORTS FC 26 (Jan 24)
```

**`buildSequentialConnections()`**: Connects all nodes linearly i→i+1, simulating an auto-play queue.

**Complexity**:
| Operation   | Complexity |
|-------------|------------|
| Add node    | O(1)       |
| Add edge    | O(1)       |
| Display     | O(V + E)   |

---

### 4.4 Stack — `WatchHistory`

**Purpose**: Record viewed videos with LIFO (Last In First Out) access — most recently watched video appears first.

**Implementation**: `std::vector<string>` used as a stack. Back of vector = top of stack.

```
push("Tekken 7")
push("EA SPORTS FC 26 Jan")
push("EA SPORTS FC 26 Jan 24")

display() → Jan 24 version first, Tekken 7 last  (LIFO)
```

**Complexity**: All operations O(1) amortized.

---

### 4.5 Queue — `Viewer::playlist`

**Purpose**: Manage a playlist with FIFO (First In First Out) order — first video added plays first.

**Implementation**: `std::queue<string>` (backed by `std::deque`).

```
enqueue("Tekken 7")         → playlist: [Tekken 7]
enqueue("EA Jan")           → playlist: [Tekken 7, EA Jan]
dequeue (play next)         → plays "Tekken 7", playlist: [EA Jan]
```

**Complexity**: `push()` and `pop()` both O(1).

---

## 5. Class Reference

### `Account` (abstract)
| Member | Type | Description |
|--------|------|-------------|
| `username` | `string` (private) | Account username |
| `password` | `string` (private) | Account password |
| `role` | `string` (private) | "creator" or "viewer" |
| `getUsername()` | method | Returns username |
| `getPassword()` | method | Returns password |
| `getRole()` | method | Returns role string |
| `displayMenu()` | pure virtual | Must be overridden |

### `Creator : public Account`
| Member | Type | Description |
|--------|------|-------------|
| `videoLibrary` | `VideoLibrary&` (private ref) | Shared video storage |
| `categoryTree` | `CategoryTree&` (private ref) | Shared AVL category tree |
| `videoGraph` | `VideoGraph&` (private ref) | Shared video graph |
| `scanFolder()` | private method | Scans a folder for video files |
| `uploadVideo()` | public | Scan folder and store a video |
| `viewVideos()` | public | List all uploaded videos |
| `addCategory()` | public | Insert a category into AVL tree |
| `uploadVideoToCategory()` | public | Attach a video to a category |
| `viewCategoryVideos()` | public | Show videos in a category |
| `viewVideoConnections()` | public | Build + display video graph |
| `displayMenu()` | override | Creator-specific colored menu |

### `Viewer : public Account`
| Member | Type | Description |
|--------|------|-------------|
| `videoLibrary` | `VideoLibrary&` (private ref) | Shared video storage |
| `watchHistory` | `WatchHistory&` (private ref) | Shared watch stack |
| `playlist` | `std::queue<string>` (private) | Viewer-owned playlist queue |
| `browseVideos()` | public | List all videos with details |
| `watchVideo()` | public | Pick by number → open in player → push to stack |
| `likeVideo()` | public | Increment likes on a video |
| `commentOnVideo()` | public | Add a comment to a video |
| `viewWatchHistory()` | public | Display stack (LIFO) |
| `addToPlaylist()` | public | Enqueue video into playlist |
| `viewPlaylist()` | public | Display queue contents |
| `playNextInPlaylist()` | public | Dequeue + open + push to stack |
| `displayMenu()` | override | Viewer-specific colored menu |

### `Video`
| Member | Description |
|--------|-------------|
| `title` | Video title (filename without extension) |
| `filePath` | Full absolute path to the video file |
| `uploader` | Username of uploader (or "System" for auto-loaded) |
| `likes` | Integer like count |
| `comments` | `vector<string>` of comment strings |

### `VideoLibrary` (HashMap)
Custom hash table with 100 buckets and separate chaining. Stores `Video` objects keyed by title.

### `CategoryTree` (AVL Tree)
Self-balancing BST. Each node stores a category name and a `vector<string>` of file paths belonging to that category.

### `VideoGraph` (Graph)
Edge-list graph. Nodes are video titles; edges are `(from_index, to_index)` pairs representing "related" or "watch-next" relationships.

### `WatchHistory` (Stack)
LIFO stack backed by `std::vector`. Displays most recently watched video first.

---

## 6. File Structure

```
StreamifyFixed/
│
├── Account.h             ← Abstract base class header
├── Account.cpp           ← Base class implementation
│
├── Creator.h             ← Creator subclass header
├── Creator.cpp           ← Creator implementation (OOP: Inheritance, Polymorphism)
│
├── Viewer.h              ← Viewer subclass header
├── Viewer.cpp            ← Viewer implementation (DSA: Stack, Queue)
│
├── Video.h               ← Video entity header
├── Video.cpp             ← Video entity implementation (OOP: Encapsulation)
│
├── VideoLibrary.h        ← HashMap header
├── VideoLibrary.cpp      ← HashMap implementation (DSA: Hash Table)
│
├── Category.h            ← AVL Tree header
├── Category.cpp          ← AVL Tree implementation (DSA: Self-Balancing BST)
│
├── VideoGraph.h          ← Graph header
├── VideoGraph.cpp        ← Graph implementation (DSA: Edge-list Graph)
│
├── WatchHistory.h        ← Stack header
├── WatchHistory.cpp      ← Stack implementation (DSA: LIFO Stack)
│
├── UI.h                  ← Header-only UI toolkit (ANSI colors, box-drawing, ShellExecute)
│
├── main.cpp              ← Entry point (auto-load, session loops, colored UI)
│
├── README.md             ← Quick-start README
├── DOCUMENTATION.md      ← This file — full project documentation
│
├── videos/               ← Place your .mp4/.mkv/.avi/.wmv files here
│   └── (video files)
│
└── .vscode/
    ├── tasks.json        ← VSCode build task (Ctrl+Shift+B)
    └── c_cpp_properties.json ← IntelliSense config
```

---

## 7. UI System

Streamify uses a custom header-only UI toolkit (`UI.h`) that provides a rich terminal interface.

### Color System (`Color` namespace)
Uses ANSI escape codes enabled via `SetConsoleMode` with `ENABLE_VIRTUAL_TERMINAL_PROCESSING`:

| Constant | Color | Used For |
|----------|-------|---------|
| `Color::CYAN` | Cyan | Creator menus, section headers |
| `Color::YELLOW` | Yellow | Video titles, menu numbers |
| `Color::GREEN` | Green | Success messages (✔), watch action |
| `Color::RED` | Red | Error messages (✘), logout |
| `Color::MAGENTA` | Magenta | Viewer menus, playlist |
| `Color::BLUE` | Blue | Watch history, graph |
| `Color::GRAY` | Gray | File paths, secondary info |

### UI Helper Functions

```cpp
printBanner()            // Draws the double-line ╔═╗ Streamify header
printHeader(title, color)// Draws a ┌─┐ section box with title
printOption(n, label)    // Prints "  [n] label" with colors
ok("message")            // Prints "✔  message" in green
err("message")           // Prints "✘  message" in red
info("message")          // Prints "ℹ  message" in gray
prompt("label")          // Prints "▶  label: " in magenta
```

### Box-Drawing Characters (`Box` namespace)
Uses UTF-8 encoded box-drawing characters. Requires `SetConsoleOutputCP(CP_UTF8)` at startup (handled in `main.cpp`).

---

## 8. Video Playback Integration

When a Viewer selects "Watch a Video" or "Play Next in Playlist", Streamify opens the video file in the **system's default media player** using Windows' `ShellExecuteW` API:

```cpp
// UI.h — openWithDefaultApp()
bool openWithDefaultApp(const std::string& path) {
    std::wstring wpath(path.begin(), path.end());
    HINSTANCE result = ShellExecuteW(
        nullptr,        // parent window (none — console app)
        L"open",        // verb: open with default handler
        wpath.c_str(),  // the file path
        nullptr,        // no extra parameters
        nullptr,        // default directory
        SW_SHOWNORMAL   // show the player window normally
    );
    return (reinterpret_cast<intptr_t>(result) > 32); // >32 = success
}
```

**Supported formats**: `.mp4`, `.mkv`, `.avi`, `.wmv`

**What opens the file**:
- If **VLC** is the default player for `.mp4` → VLC opens
- If **Windows Media Player** is default → WMP opens
- If no default is set → Windows will prompt to choose an app

**Auto-load at startup** (`main.cpp → autoLoadVideos()`):
```cpp
// Scans ./videos/ and inserts every video file into VideoLibrary + VideoGraph
for (const auto& entry : fs::directory_iterator("videos")) {
    Video v(title, filepath, "System");
    lib.insert(title, v);
    graph.addNode(title);
}
graph.buildSequentialConnections(); // links videos in a chain
```

---

## 9. How to Build & Run

### Prerequisites

| Requirement | Details |
|------------|---------|
| OS | Windows 10 / 11 |
| Compiler | g++ with C++17 support (MinGW UCRT64 / MSYS2) |
| IDE | Visual Studio Code (optional but recommended) |
| Media Player | VLC, Windows Media Player, or any default `.mp4` player |

**Check if g++ is installed:**
```powershell
g++ --version
```

**Install MSYS2/MinGW if needed:**  
→ https://www.msys2.org/  
→ After installing: `pacman -S mingw-w64-ucrt-x86_64-gcc`

---

### Method 1 — VSCode (Recommended)

1. Open the `StreamifyFixed` folder in VSCode
2. Press **`Ctrl + Shift + B`** → runs the pre-configured build task
3. Output: `Streamify.exe` in the project root
4. Open the VSCode terminal (**`` Ctrl + ` ``**) and run:

```powershell
.\Streamify.exe
```

---

### Method 2 — PowerShell

```powershell
cd "e:\GIKI\SE SPRING 2026\OOP(repeat)\Project\StreamifyFixed"

g++ -std=c++17 -g `
    Account.cpp Video.cpp VideoLibrary.cpp VideoGraph.cpp `
    WatchHistory.cpp Category.cpp Creator.cpp Viewer.cpp main.cpp `
    -o Streamify.exe

.\Streamify.exe
```

> **Note**: `-std=c++17` is required for `std::filesystem`.  
> The compiler at `E:\c++\Compiler\ucrt64\bin\g++.exe` is configured in `.vscode/tasks.json`.

---

### Adding Your Own Videos

1. Place `.mp4`, `.mkv`, `.avi`, or `.wmv` files into the `./videos/` folder
2. Run `Streamify.exe` — they are automatically detected and loaded
3. Login as Viewer → "Browse Videos" to see them, "Watch a Video" to play

---

## 10. Feature Walkthrough

### Creator Flow

```
Main Menu → [1] Creator Login
              → Enter username & password
              → Creator Menu appears:
                  [1] Upload Video from Folder
                      → Enter a folder path
                      → Shows list of .mp4/.mkv/.avi/.wmv files
                      → Select one → stored in VideoLibrary (HashMap)
                  [2] View Uploaded Videos
                      → Iterates HashMap, shows title + likes + comments
                  [3] Add Category (AVL Tree insert)
                      → Enter category name → inserted into CategoryTree
                  [4] Upload Video to Category
                      → Enter category name + folder path
                      → Attaches a video file path to the AVL node
                  [5] View Category Videos
                      → Enter category → AVL search → display file paths
                  [6] View Video Connections (Graph)
                      → Enter folder → scans files → builds VideoGraph
                      → Displays: Video A → Video B → Video C
                  [7] Logout
```

### Viewer Flow

```
Main Menu → [2] Viewer Login
              → Enter username & password
              → Viewer Menu appears:
                  [1] Browse Videos
                      → Iterates VideoLibrary (HashMap), shows all videos
                  [2] Watch a Video ← OPENS IN MEDIA PLAYER
                      → Numbered list → pick number
                      → ShellExecuteW opens video in VLC/WMP
                      → Video pushed to WatchHistory (Stack)
                  [3] Like a Video
                      → Pick from numbered list → increments like count
                  [4] Comment on a Video
                      → Pick video → type comment → stored in Video object
                  [5] View Watch History (Stack — LIFO)
                      → Displays most recently watched first
                  [6] Add to Playlist (Queue — enqueue)
                      → Pick from list → added to FIFO playlist
                  [7] View Playlist
                      → Shows queue in play order
                  [8] Play Next in Playlist (Queue — dequeue)
                      → Removes front of queue → opens in media player
                      → Also pushed to WatchHistory (Stack)
                  [9] Logout
```

---

## 11. Known Limitations

| Limitation | Explanation |
|------------|-------------|
| **No persistence** | All data (likes, comments, categories, history) is lost when the program exits. No file/database storage. |
| **No authentication** | Any username/password is accepted — no actual user registry. |
| **Single session** | Only one Creator or Viewer is active at a time. |
| **Windows only** | `ShellExecuteW` and `ANSI via SetConsoleMode` are Windows APIs. Linux/macOS would need `xdg-open`/`open` instead. |
| **Path encoding** | Video paths with non-ASCII characters may not open correctly via the `wstring` conversion used in `openWithDefaultApp()`. |
| **Graph is sequential only** | `buildSequentialConnections()` always creates a simple chain. No content-based recommendation logic. |

---

*Streamify — OOP Semester Project, GIKI*  
*Built with C++17 · g++ (MinGW UCRT64) · VSCode*
