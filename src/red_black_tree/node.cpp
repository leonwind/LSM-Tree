#include "node.hpp"


node::node(entry pair) {
    this->pair = pair;
    
    left = NULL;
    right = NULL;
    parent = NULL;

    color = RED;
}

void node::insert(entry new_pair) {
    bst_insertion(new_pair);
    fix_violations();
}

/**
 * Insert a new key-value pair into the red-black tree 
 * using a standard binary search tree insertion.
 */
void node::bst_insertion(entry new_pair) {
    if (new_pair < pair) {
        if (left == NULL) {
            node* left_child = new node(new_pair);
            left_child->parent = this;
            left = left_child;
        } else {
            left->bst_insertion(new_pair);
        }
    } else {
        if (right == NULL) {
            node* right_child = new node(new_pair);
            right_child->parent = this;
            right = right_child;
        } else {
            right->bst_insertion(new_pair);
        }
    }
}

/**
 * Check if all the Red Black Tree invariants still hold after a new insertion.
 * If not, rebalance the tree.
 */
void node::fix_violations() {

}

/**
 *   x                          y
 *  / \                        / \
 * a  y  x.rotate_left() -->  x  c
 *   / \                     / \
 *  b  c                    a  b
 */
void node::rotate_left() {
    node* new_root = right;
    new_root->parent = parent;

    node* left_child = new node(pair);
    left_child->left = left;
    left_child->right = new_root->left;
    left_child->parent = new_root;

    // set this to new_root 
    pair = new_root->pair;
    left = left_child;
    right = new_root->right;
    parent = new_root->parent;
}

/**
 *     x                           y
 *    / \                         / \
 *   y  c  x.rotate_right() -->  a  x
 *  / \                            / \
 * a  b                           b  c
 */
void node::rotate_right() {
    node* new_root = left;
    new_root->parent = parent;

    node* right_child = new node(pair);
    right_child->left = new_root->right;
    right_child->right = right;
    right_child->parent = new_root;

    // set this to new_root 
    pair = new_root->pair;
    left = new_root->left;
    right = right_child; 
    parent = new_root->parent;
}

void node::recolor() {
    if (color == RED) {
        color = BLACK;
    } else {
        color = RED;
    }
}