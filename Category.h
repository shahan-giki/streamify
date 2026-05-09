// ============================================================================
// Category.h — AVL Tree-based Category Management
// OOP Concept: Encapsulation — AVL Tree DSA wrapped inside a class;
//              internal node structure and balancing logic are fully hidden
//              from the outside. Callers interact only via the public interface.
// ============================================================================
#ifndef CATEGORY_H
#define CATEGORY_H

#include <iostream>
#include <string>
#include <vector>

class CategoryTree {
private:
    // -------------------------------------------------------------------------
    // OOP: Encapsulation — AVLNode is a private nested struct.
    // It is an implementation detail; no code outside this class can access it.
    // -------------------------------------------------------------------------
    struct AVLNode {
        std::string              category;
        std::vector<std::string> filePaths; // Videos belonging to this category
        AVLNode*                 left;
        AVLNode*                 right;
        int                      height;

        explicit AVLNode(const std::string& cat)
            : category(cat), left(nullptr), right(nullptr), height(1) {}
    };

    AVLNode* root; // OOP: Encapsulation — root is private; outside code cannot
                   //      corrupt the tree by manipulating it directly.

    // -------------------------------------------------------------------------
    // OOP: Encapsulation — all balancing/search helpers are private.
    // The "how" of the AVL tree is hidden; only the "what" is exposed publicly.
    // -------------------------------------------------------------------------
    int      getHeight(AVLNode* node) const;
    int      getBalance(AVLNode* node) const;
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* insert(AVLNode* node, const std::string& category);
    AVLNode* findNode(AVLNode* node, const std::string& category) const;
    void     inorder(AVLNode* node) const;
    void     destroyTree(AVLNode* node);

public:
    // -------------------------------------------------------------------------
    // OOP: Encapsulation — public interface is the only way to interact with
    //      the tree. Implementation can change without affecting callers.
    // -------------------------------------------------------------------------
    CategoryTree();
    ~CategoryTree();

    void                     addCategory(const std::string& category);
    bool                     addFilePathToCategory(const std::string& category,
                                                   const std::string& filePath);
    bool                     categoryExists(const std::string& category) const;
    std::vector<std::string> getFilePaths(const std::string& category) const;
    void                     displayAllCategories() const;
    void                     displayCategoryVideos(const std::string& category) const;
};

#endif // CATEGORY_H