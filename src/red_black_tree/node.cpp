#include "node.hpp"
#include <iostream>
#include <ostream>
#include <string>


node::node(entry pair, bool is_root = false) {
    this->pair = pair;
    
    left = NULL;
    right = NULL;
    parent = NULL;
    color = is_root ? BLACK : RED;
}

node::node(node* ptr) {
    pair = ptr->pair;
    color = ptr->color;
    
    left = ptr->left;
    if (left != NULL) {
        left->parent = this;
    }

    right = ptr->right;
    if (right != NULL) {
        right->parent = this;
    }

    parent = ptr->parent;
}

void node::delete_tree() {
    if (left != NULL) {
        left->delete_tree();
    } 

    if (right != NULL) {
        right->delete_tree();
    }

    delete this;
}

/**
 * Check if the key of the kv-pair exists in the tree.
 * If so, return the node, otherwise NULL.
 */
node* node::find_node(entry target) {
    if (pair == target) {
        return this;
    }
     
    if (left != NULL && pair > target) {
        return left->find_node(target);
    }

    if (right != NULL && pair < target) {
        return right->find_node(target);
    }

    return NULL;
}

/**
 * Insert a new key value pair in the tree.
 * Run first a normal bst insertion and then fix the tree
 * by fixing any violated Red Black Tree invariations.
*/
void node::insert(entry new_pair) {
    node* inserted = insert_node(new_pair);
    inserted->fix_insert();
    color = BLACK;
}

/**
 * Insert a new key-value pair into the Red Black Tree 
 * using a standard binary search tree insertion.
 * Return a pointer to inserted node.
 */
node* node::insert_node(entry new_pair) {
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
            return left->insert_node(new_pair);
        }
    } else {
        if (right == NULL) {
            node* right_child = new node(new_pair);
            right_child->parent = this;
            right = right_child;
            return right_child;
        } else {
            return right->insert_node(new_pair);
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

    while (get_color(curr->parent) == RED) {

        if (curr->parent == curr->parent->parent->left) {
            node* uncle = curr->parent->parent->right;

            if (get_color(uncle) == RED) {
                curr->parent->color = BLACK;
                uncle->color = BLACK;
                curr->parent->parent->color = RED;
                curr = curr->parent->parent;
            } else {
                if (curr == curr->parent->right) {
                    curr = curr->parent;
                    curr->rotate_left();
                    curr = curr->left;
                }
                curr->parent->color = BLACK;

                curr->parent->parent->color = RED;
                curr->parent->parent->rotate_right();
            }
        } else {
            node* uncle = curr->parent->parent->left;

            if (get_color(uncle) == RED) {
                curr->parent->color = BLACK;
                uncle->color = BLACK;
                curr->parent->parent->color = RED;
                curr = curr->parent->parent;
            } else {
                if (curr == curr->parent->left) {
                    curr = curr->parent;
                    curr->rotate_right();
                    curr = curr->right;
                }
                curr->parent->color = BLACK;
                curr->parent->parent->color = RED;
                curr->parent->parent->rotate_left();
            }
        }
    }
}

/**
 * Remove a key value pair in the tree.
 * First do a binary search to find the node to actually delete but only return the position.
 * Do the actual deletion while fixing the tree.
 * Returns if the whole tree got deleted.
*/
bool node::remove(entry target) {
    node* to_remove = find_node(target);
    if (to_remove == NULL) {
        return false;
    }

    // If to_remove has both children, we swap it with its inorder successor
    // which is a leaf and thus must have 0 or 1 children.
    if (to_remove->left != NULL && to_remove->right != NULL) {
        node* next_greater = to_remove->right->find_min();
        to_remove->pair = next_greater->pair;
        to_remove = next_greater;
    }

    if (to_remove->parent == NULL && to_remove->left == NULL && to_remove->right == NULL) {
        delete_tree();
        return true;
    }

    to_remove->remove_node();
    color = BLACK;
    return false;
}

/**
 * Check if all the Red Black Tree invariants still hold after a new insertion.
 * If not, fix them by rotating and recoloring the nodes.
 * See: https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/
 */
void node::remove_node() {
    if (color == RED || get_color(left) == RED || get_color(right) == RED) {
        node* child = (left != NULL) ? left : right;

        if (parent == NULL) {
            pair = child->pair;
            left = child->left;
            right = child->right;
            if (child->left != NULL) {
                child->left->parent = this;
            }

            if (child->right != NULL) {
                child->right->parent = this;
            }
            return; 
        }

        if (this == parent->left) {
            parent->left = child;
        } else {
            parent->right = child;
        }

        if (child != NULL) {
            child->parent = parent;
            child->color = BLACK;
        }

        delete this;
        return;
    }

    node* sibling = NULL;
    node* curr = this;
    curr->color = DOUBLE_BLACK;

    while (curr->parent != NULL && curr->color == DOUBLE_BLACK) {
        if (curr == curr->parent->left) {
            sibling = curr->parent->right;

            if (get_color(sibling) == RED) {
                sibling->color = BLACK;
                curr->parent->color = RED;
                curr->parent->rotate_left();
            } else {
                // Check for NULL of sibling->left and right??
                if (get_color(sibling->left) == BLACK && get_color(sibling->right) == BLACK) {
                    set_color(sibling, RED);

                    if (get_color(curr->parent) == RED) {
                        set_color(curr->parent, BLACK);
                    }  else {
                        set_color(curr->parent, DOUBLE_BLACK);
                    }
                    curr = curr->parent;
                } else {
                    if (get_color(sibling->right) == BLACK) {
                        set_color(sibling->left, BLACK);
                        set_color(sibling, RED);
                        sibling->rotate_right();
                        sibling = curr->parent->right;
                    }
                    set_color(sibling, curr->parent->color);
                    set_color(curr->parent, BLACK);
                    set_color(sibling->right, BLACK);
                    curr->parent->rotate_left();
                    break;
                }
            }
        } else {
            sibling = curr->parent->left;

            if (get_color(sibling) == RED) {
                sibling->color = BLACK;
                curr->parent->color = RED;
                curr->parent->rotate_left();
            } else {
                // Check for NULL of sibling->left and right??
                if (get_color(sibling->left) == BLACK && get_color(sibling->right) == BLACK) {
                    set_color(sibling, RED);
                    if (get_color(curr->parent) == RED) {
                        set_color(curr->parent, BLACK);
                    }  else {
                        set_color(curr->parent, DOUBLE_BLACK);
                    }
                    curr = curr->parent;
                } else {
                    if (get_color(sibling->left) == BLACK) {
                        set_color(sibling->right, BLACK);
                        set_color(sibling, RED);
                        sibling->rotate_left();
                        sibling = curr->parent->left;
                    }
                    set_color(sibling, curr->parent->color);
                    set_color(curr->parent, BLACK);
                    set_color(sibling->left, BLACK);
                    curr->parent->rotate_right();
                    break;
                }
            }
        }
    }

    if (parent == NULL) {
        delete this;
        return;
    }

    if (parent->left != NULL && this == parent->left) {
        parent->left = NULL;
    } else if (parent->right != NULL && this == parent->right) {
        parent->right = NULL;
    }

    delete this;
}

/**
 * Return all stored key-values pairs in an inorder fashion.
 */
void node::in_order(std::vector<entry>& nodes) const {
    if (left != NULL) {
        left->in_order(nodes);
    } 

    nodes.push_back(pair);

    if (right != NULL) {
        right->in_order(nodes);
    }
}

/*
 * Find the minimum node in the tree / the inorder successor of "this".
 */
node* node::find_min() {
    node* tmp = this;
    
    while (tmp->left != NULL) {
        tmp = tmp->left;
    } 

    return tmp;
}

/**
 * Set curr to the new color if curr is not NULL
 */
void node::set_color(node* curr, colors new_color) {
    if (curr != NULL) {
        curr->color = new_color;
    }
}

/*
 * Return the color of the node.
 * All NULL nodes are default BLACK
 */
node::colors node::get_color(node* curr) {
    if (curr == NULL) {
        return BLACK;
    } 
    return curr->color;
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
    if (left != NULL) {
        //left_child->left = new node(left);
        left_child->left = left;
        left_child->left->parent = left_child;
        //delete left;
    }
    
    left_child->right = y->left;
    if (left_child->right != NULL) {
        left_child->right->parent = left_child;
    }

    left_child->parent = this;
    left_child->color = color;
    
    if (right->right != NULL) {
        right->right->parent = this;
    }

    pair = y->pair;
    color = y->color;
    left = left_child;
    right = y->right;
    if (right != NULL) {
        right->parent = this;
    }

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
    if (right != NULL) {
        //right_child->right = new node(right);
        right_child->right = right;
        right_child->right->parent = right_child;
        //delete right;
    }

    right_child->left = y->right;
    if (right_child->left != NULL) {
        right_child->left->parent = right_child;
    }

    right_child->parent = this;
    right_child->color = color;

    if (left->left != NULL) {
        left->left->parent = this;
    }

    pair = y->pair;
    color = y->color;
    right = right_child; 
    left = y->left;
    if (left != NULL) {
        left->parent = this;
    }
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

void node::check_for_errors(bool is_root) {
    if (right != NULL) {
        right->check_for_errors(false);
    }

    if (left != NULL) {
        left->check_for_errors(false);
    }

    if (!is_root) {
        if (parent->pair.key.length() > 100) {
            std::cout << "---------" << std::endl;
            std::cout << "ERROR KEY IS TOO LONG" << std::endl;
            std::cout << "CURR: " << pair.key << std::endl;
            std::cout << "------" << std::endl;
            std::cout << "PARENT LENGTH: " << parent->pair.key.length() << std::endl;
        }
        if (parent->left != this && parent->right != this) {
            std::cout << "---------" << std::endl;
            std::cout << "ERROR ERROR ERROR" << std::endl;
            std::cout << "CURR: " << to_str() << std::endl;
            std::cout << "Parent: " << parent->to_str() << std::endl;
            std::cout << "---------" << std::endl;
        }
    }
}

std::string node::to_str() const {
    return pair.key + ":" + std::to_string(color);
}