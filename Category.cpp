// ============================================================================
// Category.cpp — AVL Tree-based Category Management (implementation)
// OOP Concept: Encapsulation — every private method here is invisible to
//              callers. The class exposes a clean public interface while
//              hiding all tree-balancing complexity inside the .cpp file.
// ============================================================================
#include "Category.h"
#include <algorithm>

using std::cout;
using std::string;
using std::vector;

// ============================================================================
// Constructor & Destructor
// OOP: Encapsulation — the constructor guarantees a valid initial state
//      (root = nullptr) so the object is always in a consistent state.
// ============================================================================

CategoryTree::CategoryTree() : root(nullptr) {}

// OOP: Encapsulation — resource cleanup is handled inside the class itself.
//      The caller never needs to manually free tree memory.
CategoryTree::~CategoryTree() {
    destroyTree(root);
}

// Private: post-order deletion prevents dangling pointers
void CategoryTree::destroyTree(AVLNode* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

// ============================================================================
// Private AVL Helpers
// OOP: Encapsulation — these methods are private. Changing the balancing
//      strategy in the future does not affect any code outside this class.
// ============================================================================

// Returns 0 for a null node, eliminating the need for null checks in callers
int CategoryTree::getHeight(AVLNode* node) const {
    return node ? node->height : 0;
}

// Positive value = left-heavy, negative = right-heavy, 0 = balanced
int CategoryTree::getBalance(AVLNode* node) const {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

// Fixes a Left-Left imbalance by rotating the subtree root to the right
CategoryTree::AVLNode* CategoryTree::rotateRight(AVLNode* y) {
    AVLNode* x  = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left  = T2;

    y->height = std::max(getHeight(y->left),  getHeight(y->right))  + 1;
    x->height = std::max(getHeight(x->left),  getHeight(x->right))  + 1;

    return x; // x is now the new subtree root
}

// Fixes a Right-Right imbalance by rotating the subtree root to the left
CategoryTree::AVLNode* CategoryTree::rotateLeft(AVLNode* x) {
    AVLNode* y  = x->right;
    AVLNode* T2 = y->left;

    y->left  = x;
    x->right = T2;

    x->height = std::max(getHeight(x->left),  getHeight(x->right))  + 1;
    y->height = std::max(getHeight(y->left),  getHeight(y->right))  + 1;

    return y; // y is now the new subtree root
}

// Private recursive insert — handles all four AVL imbalance cases
CategoryTree::AVLNode* CategoryTree::insert(AVLNode* node, const string& category) {
    // Standard BST insert
    if (!node)
        return new AVLNode(category);

    if (category < node->category)
        node->left  = insert(node->left,  category);
    else if (category > node->category)
        node->right = insert(node->right, category);
    else
        return node; // OOP: Encapsulation — duplicate silently ignored inside class

    // Update height before checking balance
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1  && category < node->left->category)   // Left-Left
        return rotateRight(node);

    if (balance < -1 && category > node->right->category)  // Right-Right
        return rotateLeft(node);

    if (balance > 1  && category > node->left->category) { // Left-Right
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && category < node->right->category) { // Right-Left
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// Private O(log n) search — only used internally by public methods
CategoryTree::AVLNode* CategoryTree::findNode(AVLNode* node, const string& category) const {
    if (!node || node->category == category)
        return node;
    return (category < node->category)
        ? findNode(node->left,  category)
        : findNode(node->right, category);
}

// Private in-order traversal — prints categories alphabetically
void CategoryTree::inorder(AVLNode* node) const {
    if (!node) return;
    inorder(node->left);
    cout << " Category: " << node->category << "\n";
    if (!node->filePaths.empty()) {
        cout << "   Videos:\n";
        for (size_t i = 0; i < node->filePaths.size(); ++i)
            cout << "     " << (i + 1) << ". " << node->filePaths[i] << "\n";
    }
    inorder(node->right);
}

// ============================================================================
// Public Interface
// OOP: Encapsulation — these are the ONLY entry points for external code.
//      All public methods delegate to private helpers, keeping the interface
//      simple and the implementation details hidden.
// ============================================================================

void CategoryTree::addCategory(const string& category) {
    root = insert(root, category); // Private helper does the real work
    cout << " Category added: " << category << "\n";
}

bool CategoryTree::addFilePathToCategory(const string& category, const string& filePath) {
    AVLNode* node = findNode(root, category); // Private search, hidden from caller
    if (node) {
        node->filePaths.push_back(filePath);
        return true;
    }
    cout << " Category not found: " << category << "\n";
    return false;
}

bool CategoryTree::categoryExists(const string& category) const {
    return findNode(root, category) != nullptr;
}

// OOP: Encapsulation — returns a copy of the vector, not a reference,
//      so the caller cannot accidentally corrupt internal data.
vector<string> CategoryTree::getFilePaths(const string& category) const {
    AVLNode* node = findNode(root, category);
    return node ? node->filePaths : vector<string>{};
}

void CategoryTree::displayAllCategories() const {
    if (!root) {
        cout << " No categories available.\n";
        return;
    }
    cout << "\n === All Categories ===\n";
    inorder(root); // Delegates to private helper for alphabetical print
}

void CategoryTree::displayCategoryVideos(const string& category) const {
    AVLNode* node = findNode(root, category);
    if (!node) {
        cout << " Category not found: " << category << "\n";
        return;
    }
    if (node->filePaths.empty()) {
        cout << " No videos in category '" << category << "'.\n";
        return;
    }
    cout << " Videos in category '" << category << "':\n";
    for (size_t i = 0; i < node->filePaths.size(); ++i)
        cout << "   " << (i + 1) << ". " << node->filePaths[i] << "\n";
}