

#ifndef WATCHHISTORY_H
#define WATCHHISTORY_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class WatchHistory
{
private:
    vector<string> history;   // Stack storage

public:

    // Add video to history
    void push(string title);

    // Remove latest watched video
    void pop();

    // Show latest watched video
    void top();

    // Check if history is empty
    bool isEmpty();

    // Total videos watched
    int size();

    // Display complete history
    void display();

    // Clear all history
    void clear();
};

#endif
