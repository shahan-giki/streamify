

#include "WatchHistory.h"

using namespace std;

// Add video to stack
void WatchHistory::push(string title)
{
    history.push_back(title);

    cout << "Video added to watch history.\n";
}

// Remove latest watched video
void WatchHistory::pop()
{
    if (history.empty())
    {
        cout << "Watch history is empty.\n";
    }
    else
    {
        cout << "Removed: " << history.back() << endl;
        history.pop_back();
    }
}

// Show latest watched video
void WatchHistory::top()
{
    if (history.empty())
    {
        cout << "Watch history is empty.\n";
    }
    else
    {
        cout << "Most recent video: " << history.back() << endl;
    }
}

// Check if stack is empty
bool WatchHistory::isEmpty()
{
    return history.empty();
}

// Return total videos
int WatchHistory::size()
{
    return history.size();
}

// Display watch history
void WatchHistory::display()
{
    if (history.empty())
    {
        cout << "No videos watched yet.\n";
        return;
    }

    cout << "\n===== Watch History =====\n";

    for (int i = history.size() - 1; i >= 0; i--)
    {
        cout << history.size() - i << ". " << history[i] << endl;
    }
}

// Clear history
void WatchHistory::clear()
{
    history.clear();

    cout << "Watch history cleared.\n";
}
