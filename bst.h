#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
#include <stdexcept>

template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value& value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

template<typename Key, typename Value>
Node<Key, Value>::Node(
    const Key& key,
    const Value& value,
    Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{
}

template<typename Key, typename Value>
Node<Key, Value>::~Node()
{
}

template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    virtual void insert(const std::pair<const Key, Value>& keyValuePair);
    virtual void remove(const Key& key);
    void clear();
    bool isBalanced() const;
    void print() const;
    bool empty() const;

    template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue>& tree);

public:
    class iterator
    {
    public:
        iterator();

        std::pair<const Key, Value>& operator*() const;
        std::pair<const Key, Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key, Value>* ptr);
        Node<Key, Value>* current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;
    Value& operator[](const Key& key);
    Value const& operator[](const Key& key) const;

protected:
    Node<Key, Value>* internalFind(const Key& k) const;
    Node<Key, Value>* getSmallestNode() const;
    static Node<Key, Value>* predecessor(Node<Key, Value>* current);
    static Node<Key, Value>* successor(Node<Key, Value>* current);
    static int balancedHeight(Node<Key, Value>* node);
    static void clearSubtree(Node<Key, Value>* node);

    virtual void printRoot(Node<Key, Value>* r) const;
    virtual void nodeSwap(Node<Key, Value>* n1, Node<Key, Value>* n2);

protected:
    Node<Key, Value>* root_;
};

template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key, Value>* ptr)
{
    current_ = ptr;
}

template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator()
{
    current_ = NULL;
}

template<class Key, class Value>
std::pair<const Key, Value>&
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

template<class Key, class Value>
std::pair<const Key, Value>*
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

template<class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(
    const typename BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return current_ == rhs.current_;
}

template<class Key, class Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(
    const typename BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return current_ != rhs.current_;
}

template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    current_ = BinarySearchTree<Key, Value>::successor(current_);
    return *this;
}

template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() :
    root_(NULL)
{
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    clear();
}

template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    iterator result(getSmallestNode());
    return result;
}

template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    iterator result(NULL);
    return result;
}

template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key& key) const
{
    Node<Key, Value>* current = internalFind(key);
    iterator result(current);
    return result;
}

template<class Key, class Value>
Value& BinarySearchTree<Key, Value>::operator[](const Key& key)
{
    Node<Key, Value>* current = internalFind(key);
    if (current == NULL) {
        throw std::out_of_range("Invalid key");
    }
    return current->getValue();
}

template<class Key, class Value>
const Value& BinarySearchTree<Key, Value>::operator[](const Key& key) const
{
    Node<Key, Value>* current = internalFind(key);
    if (current == NULL) {
        throw std::out_of_range("Invalid key");
    }
    return current->getValue();
}

template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(
    const std::pair<const Key, Value>& keyValuePair)
{
    if (root_ == NULL) {
        root_ = new Node<Key, Value>(
            keyValuePair.first, keyValuePair.second, NULL);
        return;
    }

    Node<Key, Value>* current = root_;

    while (true) {
        if (keyValuePair.first < current->getKey()) {
            if (current->getLeft() != NULL) {
                current = current->getLeft();
            } else {
                current->setLeft(new Node<Key, Value>(
                    keyValuePair.first, keyValuePair.second, current));
                return;
            }
        } else if (current->getKey() < keyValuePair.first) {
            if (current->getRight() != NULL) {
                current = current->getRight();
            } else {
                current->setRight(new Node<Key, Value>(
                    keyValuePair.first, keyValuePair.second, current));
                return;
            }
        } else {
            current->setValue(keyValuePair.second);
            return;
        }
    }
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    Node<Key, Value>* node = internalFind(key);
    if (node == NULL) {
        return;
    }

    if (node->getLeft() != NULL && node->getRight() != NULL) {
        nodeSwap(node, predecessor(node));
    }

    Node<Key, Value>* child = NULL;
    if (node->getLeft() != NULL) {
        child = node->getLeft();
    } else {
        child = node->getRight();
    }

    Node<Key, Value>* parent = node->getParent();

    if (child != NULL) {
        child->setParent(parent);
    }

    if (parent == NULL) {
        root_ = child;
    } else if (parent->getLeft() == node) {
        parent->setLeft(child);
    } else {
        parent->setRight(child);
    }

    delete node;
}

template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    if (current == NULL) {
        return NULL;
    }

    if (current->getLeft() != NULL) {
        current = current->getLeft();
        while (current->getRight() != NULL) {
            current = current->getRight();
        }
        return current;
    }

    Node<Key, Value>* parent = current->getParent();
    while (parent != NULL && current == parent->getLeft()) {
        current = parent;
        parent = parent->getParent();
    }
    return parent;
}

template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::successor(Node<Key, Value>* current)
{
    if (current == NULL) {
        return NULL;
    }

    if (current->getRight() != NULL) {
        current = current->getRight();
        while (current->getLeft() != NULL) {
            current = current->getLeft();
        }
        return current;
    }

    Node<Key, Value>* parent = current->getParent();
    while (parent != NULL && current == parent->getRight()) {
        current = parent;
        parent = parent->getParent();
    }
    return parent;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clearSubtree(Node<Key, Value>* node)
{
    if (node == NULL) {
        return;
    }

    clearSubtree(node->getLeft());
    clearSubtree(node->getRight());
    delete node;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    clearSubtree(root_);
    root_ = NULL;
}

template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    Node<Key, Value>* current = root_;

    if (current == NULL) {
        return NULL;
    }

    while (current->getLeft() != NULL) {
        current = current->getLeft();
    }
    return current;
}

template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    Node<Key, Value>* current = root_;

    while (current != NULL) {
        if (key < current->getKey()) {
            current = current->getLeft();
        } else if (current->getKey() < key) {
            current = current->getRight();
        } else {
            return current;
        }
    }

    return NULL;
}

template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::balancedHeight(Node<Key, Value>* node)
{
    if (node == NULL) {
        return 0;
    }

    int leftHeight = balancedHeight(node->getLeft());
    if (leftHeight == -1) {
        return -1;
    }

    int rightHeight = balancedHeight(node->getRight());
    if (rightHeight == -1) {
        return -1;
    }

    if (leftHeight - rightHeight > 1 ||
        rightHeight - leftHeight > 1) {
        return -1;
    }

    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    return balancedHeight(root_) != -1;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap(
    Node<Key, Value>* n1,
    Node<Key, Value>* n2)
{
    if ((n1 == n2) || (n1 == NULL) || (n2 == NULL)) {
        return;
    }

    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if (n1p != NULL && (n1 == n1p->getLeft())) {
        n1isLeft = true;
    }

    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if (n2p != NULL && (n2 == n2p->getLeft())) {
        n2isLeft = true;
    }

    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if (n1r != NULL && n1r == n2) {
        n2->setRight(n1);
        n1->setParent(n2);
    } else if (n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);
    } else if (n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);
    } else if (n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);
    }

    if (n1p != NULL && n1p != n2) {
        if (n1isLeft) {
            n1p->setLeft(n2);
        } else {
            n1p->setRight(n2);
        }
    }

    if (n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }

    if (n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if (n2p != NULL && n2p != n1) {
        if (n2isLeft) {
            n2p->setLeft(n1);
        } else {
            n2p->setRight(n1);
        }
    }

    if (n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }

    if (n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }

    if (root_ == n1) {
        root_ = n2;
    } else if (root_ == n2) {
        root_ = n1;
    }
}

#include "print_bst.h"

#endif