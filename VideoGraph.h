// ============================================================================
// VideoGraph.h — Graph-based Video Connections / Related Videos
// OOP Concept: Encapsulation — Graph DSA wrapped inside its own class
// DSA: Graph (adjacency list for video relationships)
// ============================================================================
#ifndef VIDEOGRAPH_H
#define VIDEOGRAPH_H

#include <iostream>
#include <string>
#include <vector>

// OOP Concept: Encapsulation — Graph data structure wrapped in a class
// Internal adjacency list and node storage are private
class VideoGraph {
private:
    std::vector<std::string> nodes;               // Video titles as nodes
    std::vector<std::pair<int, int>> edges;        // Edge list (from, to)

public:
    // Add a video node to the graph
    void addNode(const std::string& title);

    // Add a directed edge between two video indices
    void addEdge(int from, int to);

    // Get the number of nodes
    int getNodeCount() const;

    // Get node title by index (Encapsulation — controlled access)
    std::string getNodeTitle(int index) const;

    // Display all video connections
    void displayGraph() const;

    // Build sequential connections between all videos
    void buildSequentialConnections();

    // Clear the entire graph
    void clear();
};

#endif // VIDEOGRAPH_H
