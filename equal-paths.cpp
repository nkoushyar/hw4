#ifndef RECCHECK
// Add any needed includes here.
#endif

#include "equal-paths.h"
using namespace std;

int equalPathHeight(Node* root)
{
    if (root == nullptr) {
        return 0;
    }

    int leftHeight = equalPathHeight(root->left);
    int rightHeight = equalPathHeight(root->right);

    if (leftHeight == -1 || rightHeight == -1) {
        return -1;
    }

    if (root->left != nullptr && root->right != nullptr &&
        leftHeight != rightHeight) {
        return -1;
    }

    if (root->left != nullptr) {
        return leftHeight + 1;
    }
    return rightHeight + 1;
}

bool equalPaths(Node* root)
{
    return equalPathHeight(root) != -1;
}