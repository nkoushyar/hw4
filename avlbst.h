#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    int8_t getBalance() const;
    void setBalance(int8_t balance);
    void updateBalance(int8_t diff);

    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;
};

template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(
    const Key& key,
    const Value& value,
    AVLNode<Key, Value>* parent) :
    Node<Key, Value>(key, value, parent),
    balance_(0)
{
}

template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{
}

template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}

template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert(const std::pair<const Key, Value>& new_item);
    virtual void remove(const Key& key);

protected:
    virtual void nodeSwap(AVLNode<Key, Value>* n1, AVLNode<Key, Value>* n2);

    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rebalance(AVLNode<Key, Value>* node);
    void rebalanceAfterRemoval(AVLNode<Key, Value>* node, int8_t diff);
};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* pivot = node->getRight();
    AVLNode<Key, Value>* middle = pivot->getLeft();
    AVLNode<Key, Value>* parent = node->getParent();

    if (parent == NULL) {
        this->root_ = pivot;
    } else if (parent->getLeft() == node) {
        parent->setLeft(pivot);
    } else {
        parent->setRight(pivot);
    }

    pivot->setParent(parent);
    pivot->setLeft(node);
    node->setParent(pivot);
    node->setRight(middle);

    if (middle != NULL) {
        middle->setParent(node);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* pivot = node->getLeft();
    AVLNode<Key, Value>* middle = pivot->getRight();
    AVLNode<Key, Value>* parent = node->getParent();

    if (parent == NULL) {
        this->root_ = pivot;
    } else if (parent->getLeft() == node) {
        parent->setLeft(pivot);
    } else {
        parent->setRight(pivot);
    }

    pivot->setParent(parent);
    pivot->setRight(node);
    node->setParent(pivot);
    node->setLeft(middle);

    if (middle != NULL) {
        middle->setParent(node);
    }
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rebalance(
    AVLNode<Key, Value>* node)
{
    if (node->getBalance() == -2) {
        AVLNode<Key, Value>* left = node->getLeft();

        if (left->getBalance() <= 0) {
            int8_t leftBalance = left->getBalance();
            rotateRight(node);

            if (leftBalance == 0) {
                node->setBalance(-1);
                left->setBalance(1);
            } else {
                node->setBalance(0);
                left->setBalance(0);
            }
            return left;
        }

        AVLNode<Key, Value>* pivot = left->getRight();
        int8_t pivotBalance = pivot->getBalance();

        rotateLeft(left);
        rotateRight(node);

        if (pivotBalance == -1) {
            left->setBalance(0);
            node->setBalance(1);
        } else if (pivotBalance == 1) {
            left->setBalance(-1);
            node->setBalance(0);
        } else {
            left->setBalance(0);
            node->setBalance(0);
        }

        pivot->setBalance(0);
        return pivot;
    }

    if (node->getBalance() == 2) {
        AVLNode<Key, Value>* right = node->getRight();

        if (right->getBalance() >= 0) {
            int8_t rightBalance = right->getBalance();
            rotateLeft(node);

            if (rightBalance == 0) {
                node->setBalance(1);
                right->setBalance(-1);
            } else {
                node->setBalance(0);
                right->setBalance(0);
            }
            return right;
        }

        AVLNode<Key, Value>* pivot = right->getLeft();
        int8_t pivotBalance = pivot->getBalance();

        rotateRight(right);
        rotateLeft(node);

        if (pivotBalance == 1) {
            node->setBalance(-1);
            right->setBalance(0);
        } else if (pivotBalance == -1) {
            node->setBalance(0);
            right->setBalance(1);
        } else {
            node->setBalance(0);
            right->setBalance(0);
        }

        pivot->setBalance(0);
        return pivot;
    }

    return node;
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert(
    const std::pair<const Key, Value>& new_item)
{
    if (this->root_ == NULL) {
        this->root_ = new AVLNode<Key, Value>(
            new_item.first, new_item.second, NULL);
        return;
    }

    AVLNode<Key, Value>* current =
        static_cast<AVLNode<Key, Value>*>(this->root_);

    while (true) {
        if (new_item.first < current->getKey()) {
            if (current->getLeft() != NULL) {
                current = current->getLeft();
            } else {
                AVLNode<Key, Value>* inserted =
                    new AVLNode<Key, Value>(
                        new_item.first, new_item.second, current);
                current->setLeft(inserted);
                current = inserted;
                break;
            }
        } else if (current->getKey() < new_item.first) {
            if (current->getRight() != NULL) {
                current = current->getRight();
            } else {
                AVLNode<Key, Value>* inserted =
                    new AVLNode<Key, Value>(
                        new_item.first, new_item.second, current);
                current->setRight(inserted);
                current = inserted;
                break;
            }
        } else {
            current->setValue(new_item.second);
            return;
        }
    }

    AVLNode<Key, Value>* child = current;
    AVLNode<Key, Value>* parent = child->getParent();

    while (parent != NULL) {
        if (parent->getLeft() == child) {
            parent->updateBalance(-1);
        } else {
            parent->updateBalance(1);
        }

        if (parent->getBalance() == 0) {
            break;
        }

        if (parent->getBalance() == -2 ||
            parent->getBalance() == 2) {
            rebalance(parent);
            break;
        }

        child = parent;
        parent = parent->getParent();
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rebalanceAfterRemoval(
    AVLNode<Key, Value>* node,
    int8_t diff)
{
    while (node != NULL) {
        node->updateBalance(diff);

        if (node->getBalance() == -1 ||
            node->getBalance() == 1) {
            return;
        }

        if (node->getBalance() == 0) {
            AVLNode<Key, Value>* parent = node->getParent();
            if (parent == NULL) {
                return;
            }

            diff = (parent->getLeft() == node) ? 1 : -1;
            node = parent;
            continue;
        }

        int8_t heavyChildBalance;
        if (node->getBalance() == -2) {
            heavyChildBalance = node->getLeft()->getBalance();
        } else {
            heavyChildBalance = node->getRight()->getBalance();
        }

        bool heightDecreased = (heavyChildBalance != 0);
        AVLNode<Key, Value>* newSubtreeRoot = rebalance(node);

        if (!heightDecreased) {
            return;
        }

        AVLNode<Key, Value>* parent = newSubtreeRoot->getParent();
        if (parent == NULL) {
            return;
        }

        diff = (parent->getLeft() == newSubtreeRoot) ? 1 : -1;
        node = parent;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    AVLNode<Key, Value>* node =
        static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    if (node == NULL) {
        return;
    }

    if (node->getLeft() != NULL && node->getRight() != NULL) {
        AVLNode<Key, Value>* pred =
            static_cast<AVLNode<Key, Value>*>(
                BinarySearchTree<Key, Value>::predecessor(node));
        this->nodeSwap(node, pred);
    }

    AVLNode<Key, Value>* parent = node->getParent();
    bool wasLeftChild =
        (parent != NULL && parent->getLeft() == node);

    AVLNode<Key, Value>* child =
        (node->getLeft() != NULL) ? node->getLeft() : node->getRight();

    if (child != NULL) {
        child->setParent(parent);
    }

    if (parent == NULL) {
        this->root_ = child;
    } else if (wasLeftChild) {
        parent->setLeft(child);
    } else {
        parent->setRight(child);
    }

    delete node;

    if (parent != NULL) {
        int8_t diff = wasLeftChild ? 1 : -1;
        rebalanceAfterRemoval(parent, diff);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(
    AVLNode<Key, Value>* n1,
    AVLNode<Key, Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);

    int8_t tempBalance = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempBalance);
}

#endif