# 🎬 Streamify — C++ Console Video Streaming Platform

> A C++17 console video streaming platform demonstrating **OOP** (Encapsulation, Inheritance, Polymorphism, Abstraction) and **DSA** (HashMap, AVL Tree, Graph, Stack, Queue).

---

## Quick Start

```powershell
# Build
g++ -std=c++17 -g Account.cpp Video.cpp VideoLibrary.cpp VideoGraph.cpp WatchHistory.cpp Category.cpp Creator.cpp Viewer.cpp main.cpp -o Streamify.exe

# Run
.\Streamify.exe
```

Or press **`Ctrl + Shift + B`** in VSCode, then `.\Streamify.exe`.

> Drop your `.mp4` / `.mkv` / `.avi` / `.wmv` files in `./videos/` — they auto-load at startup.

---

## OOP Concepts

| Concept | Where Used |
|---------|-----------|
| **Encapsulation** | Every class exposes only a public interface; all internals are private |
| **Inheritance** | `Creator` and `Viewer` both inherit `Account` |
| **Polymorphism** | `displayMenu()` is pure virtual in `Account`, overridden in both subclasses |
| **Abstraction** | `Account` is abstract — cannot be instantiated directly |

## DSA Concepts

| Structure | Class | Complexity |
|-----------|-------|-----------|
| **HashMap** | `VideoLibrary` | O(1) avg insert/find |
| **AVL Tree** | `CategoryTree` | O(log n) insert/search |
| **Graph** | `VideoGraph` | Edge-list, O(V+E) display |
| **Stack** | `WatchHistory` | O(1) push/pop, LIFO |
| **Queue** | `Viewer::playlist` | O(1) enqueue/dequeue, FIFO |

---

## Features

**Creator**: Upload videos, manage AVL-tree categories, view video connection graph.

**Viewer**: Browse & watch videos (opens in VLC/WMP via `ShellExecuteW`), like, comment, Stack-based watch history, Queue-based playlist.

---

## File Structure

```
Account.h/cpp        ← Abstract base (OOP: Abstraction, Polymorphism)
Creator.h/cpp        ← Creator subclass (OOP: Inheritance)
Viewer.h/cpp         ← Viewer subclass  (OOP: Inheritance + DSA: Stack, Queue)
Video.h/cpp          ← Video entity     (OOP: Encapsulation)
VideoLibrary.h/cpp   ← HashMap          (DSA: Hash Table)
Category.h/cpp       ← AVL Tree         (DSA: Self-balancing BST)
VideoGraph.h/cpp     ← Graph            (DSA: Edge-list)
WatchHistory.h/cpp   ← Stack            (DSA: LIFO)
UI.h                 ← ANSI colors + box-drawing + ShellExecuteW video launcher
main.cpp             ← Entry point + auto-load from ./videos/
videos/              ← Place your video files here
```

See **[DOCUMENTATION.md](DOCUMENTATION.md)** for the full class reference, architecture diagram, OOP/DSA deep-dives, and feature walkthrough.

---

*GIKI — OOP Semester Project · C++17 · Windows*
