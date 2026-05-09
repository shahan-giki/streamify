// ============================================================================
// VideoGraph.cpp — Graph-based Video Connections implementation
// OOP Concept: Encapsulation — Graph internal logic hidden from caller
// DSA: Graph (edge list representation)
// ============================================================================
#include "VideoGraph.h"

using std::cout;
using std::string;

void VideoGraph::addNode(const string& title) {
    nodes.push_back(title);
}

void VideoGraph::addEdge(int from, int to) {
    if (from >= 0 && from < static_cast<int>(nodes.size()) &&
        to >= 0 && to < static_cast<int>(nodes.size())) {
        edges.emplace_back(from, to);
    }
}

int VideoGraph::getNodeCount() const {
    return static_cast<int>(nodes.size());
}

string VideoGraph::getNodeTitle(int index) const {
    if (index >= 0 && index < static_cast<int>(nodes.size())) {
        return nodes[index];
    }
    return "";
}

void VideoGraph::displayGraph() const {
    if (edges.empty()) {
        cout << " No video connections found.\n";
        return;
    }
    cout << "\n === Video Connections ===\n";
    for (const auto& edge : edges) {
        cout << "   " << nodes[edge.first] << " -> " << nodes[edge.second] << "\n";
    }
}

void VideoGraph::buildSequentialConnections() {
    edges.clear();
    for (int i = 0; i < static_cast<int>(nodes.size()) - 1; ++i) {
        addEdge(i, i + 1);
    }
}

void VideoGraph::clear() {
    nodes.clear();
    edges.clear();
}
