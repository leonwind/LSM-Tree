#include "node.hpp"
#include <iostream>
#include <ostream>


node::node(entry pair, bool is_root = false) {
    this->pair = pair;
    
    left = NULL;
    right = NULL;
    parent = NULL;

    color = is_root ? BLACK : RED;
}

/**
 * Check if the key value pair target exists in the tree
 */
bool node::exists(entry target) const {
    if (pair == target) {
        return true;
    }

    if (left != NULL && pair < target) {
        return left->exists(target);
    }

    if (right != NULL && pair > target) {
        return right->exists(target);
    }

    return false;
}

/**
 * Insert a new key value pair in the tree.
 * Run first a normal bst insertion and then fix the tree
 * by fixing any violated Red Black Tree invariations.
*/
void node::insert(entry new_pair) {
    node* inserted = bst_insertion(new_pair);
    inserted->fix_insert();
    color = BLACK;
}

/**
 * Insert a new key-value pair into the red-black tree 
 * using a standard binary search tree insertion.
 * Return a pointer to inserted node.
 */
node* node::bst_insertion(entry new_pair) {
    if (new_pair == pair) {
        pair = new_pair;
        return this;
    }

    if (new_pair < pair) {
        if (left == NULL) {
            node* left_child = new node(new_pair);
            left_child->parent = this;
            left = left_child;
            return left_child;
        } else {
            return left->bst_insertion(new_pair);
        }
    } else {
        if (right == NULL) {
            node* right_child = new node(new_pair);
            right_child->parent = this;
            right = right_child;
            return right_child;
        } else {
            return right->bst_insertion(new_pair);
        }
    }
}

/**
 * Check if all the Red Black Tree invariants still hold after a new insertion.
 * If not, fix them by rotating and recoloring the nodes.
 * See https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/ 
 */
void node::fix_insert() {
    node* curr = this;

    while (curr->parent != NULL && curr->parent->color == RED) {

        if (curr->parent == curr->parent->parent->left) {
            node* uncle = curr->parent->parent->right;

            if (uncle != NULL && uncle->color == RED) {
                curr->parent->color = BLACK;
                uncle->color = BLACK;
                curr->parent->parent->color = RED;
                curr = curr->parent->parent;
            } else {
                if (curr == curr->parent->right) {
                    curr = curr->parent;
                    curr->rotate_left();
                }
                curr->parent->color = BLACK;
                curr->parent->parent->color = RED;
                curr->parent->parent->rotate_right();
            }
        } else {
            node* uncle = curr->parent->parent->left;

            if (uncle != NULL && uncle->color == RED) {
                curr->parent->color = BLACK;
                uncle->color = BLACK;
                curr->parent->parent->color = RED;
                curr = curr->parent->parent;
            } else {
                if (curr == curr->parent->left) {
                    curr = curr->parent;
                    curr->rotate_right();
                }
                curr->parent->color = BLACK;
                curr->parent->parent->color = RED;
                curr->parent->parent->rotate_left();
            }
        }
    }
}

/**
 * Remove a key vlaue pair in the tree.
 * Run first a normal bst removal and then fix the tree
 * by fixing any violated Red Black Tree invariations.
*/
void node::remove(entry target) {

}

/**
 * Check if all the Red Black Tree invariants still hold after a new insertion.
 * If not, fix them by rotating and recoloring the nodes.
 * 
 */
void node::fix_remove() {

}

/**
 * Return all stored key-values pairs in an inorder fashion.
 */
std::vector<entry> node::get_all_nodes() const {
    std::vector<entry> nodes;
    return nodes;
}

/**
 *   x                          y
 *  / \                        / \
 * a  y  x.rotate_left() -->  x  c
 *   / \                     / \
 *  b  c                    a  b
 */
void node::rotate_left() {
    node* y = right;

    node* left_child = new node(pair);
    left_child->left = left;
    left_child->right = y->left;
    left_child->parent = this;
    left_child->color = color;

    if (right->right != NULL) {
        right->right->parent = this;
    }

    pair = y->pair;
    left = left_child;
    color = y->color;
    right = y->right;
    delete y;
}

/**
 *     x                           y
 *    / \                         / \
 *   y  c  x.rotate_right() -->  a  x
 *  / \                            / \
 * a  b                           b  c
 */
void node::rotate_right() {
    node* y = left;

    node* right_child = new node(pair);
    right_child->left = y->right;
    right_child->right = right;
    right_child->parent = this;
    right_child->color = color;

    if (left->left != NULL) {
        left->left->parent = this;
    }

    // set this to y 
    pair = y->pair;
    color = y->color;
    right = right_child; 
    left = y->left;
    delete y;
}

void node::print_2d(int space) const {
    space += 4;
    if (right != NULL) {
        right->print_2d(space);
    }

    std::cout << std::endl;
    for (int i = 0; i < space; i++) {
        std::cout << " ";
    }
    std::cout << pair.key << ":" << color << std::endl;

    if (left != NULL) {
        left->print_2d(space);
    }
}

std::string node::to_str() const {
    if (color == RED) {
        return pair.key + ":" + "RED";
    } else {
        return pair.key + ":" + "BLACK";
    }
}