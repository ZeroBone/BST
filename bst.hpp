/**
MIT License

Copyright (c) 2019 Alexander Mayorov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#ifndef BST_BST_HPP
#define BST_BST_HPP

template <class T>
class BST;

template <class T>
class BSTNode {

    friend class BST<T>;

    public:

    T value;

    private:

    BSTNode* left;
    BSTNode* right;

    explicit BSTNode(T value) : value(value), left(nullptr), right(nullptr) {}

    void destroy() {

        if (left != nullptr) {
            left->destroy();
            delete left;
        }

        if (right != nullptr) {
            right->destroy();
            delete right;
        }

    }

    static void debugPrint(BSTNode* node, int &indent);

};

template <class T>
void BSTNode<T>::debugPrint(BSTNode* node, int &indent) {

    indent++;

    if (node->left != nullptr) {
        BSTNode<T>::debugPrint(node->left, indent);
    }

    for (int i = 0; i < indent - 1; i++) {
        std::cout << "    ";
    }

    std::cout << node->value << std::endl;

    if (node->right != nullptr) {
        BSTNode<T>::debugPrint(node->right, indent);
    }

    indent--;

}

template <class T>
class BST {

    private:

    BSTNode<T>* root;
    bool rightSubtree;

    public:

    explicit BST() : root(nullptr), rightSubtree(true) {}

    ~BST() {

        if (root != nullptr) {
            root->destroy();
            delete root;
        }

    }

    void debugPrint() {

        if (root == nullptr) {
            return;
        }

        int indent = 0;

        BSTNode<T>::debugPrint(root, indent);

    }

    void insertUnique(T& value) {

        if (root == nullptr) {

            root = new BSTNode<T>(value);

            return;

        }

        BSTNode<T>* current = root;

        // BSTNode<T>* destination;
        // for (;(destination = (current->value < value ? current->left : current->right)) != nullptr; current = destination);

        for (;;) {

            if (value < current->value) {

                if (current->left == nullptr) {
                    break;
                }

                current = current->left;

            }
            else {

                if (current->right == nullptr) {
                    break;
                }

                current = current->right;

            }

        }

        (value < current->value ? current->left : current->right) = new BSTNode<T>(value);

    }

    bool insert(T& value) {

        if (root == nullptr) {

            root = new BSTNode<T>(value);

            return true;

        }

        BSTNode<T>* current = root;

        for (;;) {

            if (current->value == value) {
                return false;
            }

            if (value < current->value) {

                if (current->left == nullptr) {
                    break;
                }

                current = current->left;

            }
            else {

                if (current->right == nullptr) {
                    break;
                }

                current = current->right;

            }

        }

        (value < current->value ? current->left : current->right) = new BSTNode<T>(value);

        return true;

    }

    BSTNode<T>* find(T& value) {

        if (root == nullptr) {
            return nullptr;
        }

        BSTNode<T>* current = root;

        while (value != current->value) {

            if (value < current->value) {

                if (current->left == nullptr) {
                    return nullptr; // not found
                }

                current = current->left;

            }
            else {

                if (current->right == nullptr) {
                    return nullptr; // not found
                }

                current = current->right;

            }

        }

        return current;

    }

    bool remove(T& value) {

        if (root == nullptr) {
            return false; // tree empty, obviously not found
        }

        // this is a pointer to a pointer, which we used to access current node during binary search
        // it is used to update the parent's pointer to the child without actually getting access to the parent
        // i used this approach because otherwise we would have to handle deletion of the root element separately,
        // because the root element obviously doesn't have any parent, but it has a pointer from the
        // BST descriptor, from which it is referenced
        BSTNode<T>** parentPointer = &root;

        BSTNode<T>* current = root;

        while (value != current->value) {

            if (value < current->value) {

                if (current->left == nullptr) {
                    return false; // not found
                }

                parentPointer = &current->left;
                current = current->left;

            }
            else {

                if (current->right == nullptr) {
                    return false; // not found
                }

                parentPointer = &current->right;
                current = current->right;

            }

        }

        // now we reached the node that should be deleted
        // current is pointing to the node that should be deleted

        // std::cout << "Node reached: " << current->value << std::endl;

        // first check if one of the children is missing, then we are lucky

        if (current->left == nullptr) {
            *parentPointer = current->right; // can be null, of course
            delete current;
            return true;
        }

        if (current->right == nullptr) {
            *parentPointer = current->left; // can be null, of course
            delete current;
            return true;
        }

        // we are unlucky, the element that should be deleted has both children

        // we can choose the rightmost element in the left subtree and swap it with the found element
        // or we can choose the leftmost element in the right subtree

        if (rightSubtree) {

            BSTNode<T>* tailParent = current->right;
            BSTNode<T>* tail = current->right;

            // find all the successors

            while (tail->left != nullptr) {
                tailParent = tail;
                tail = tail->left;
            }

            // we have reached the end of the row

            if (sizeof(T) <= sizeof(void*) + sizeof(void*)) {
                // copy data

                if (tail == tailParent) {
                    // short row (the right child of current doesn't have a left subtree)
                    current->right = tail->right;
                }
                else {
                    tailParent->left = tail->right;
                }

                // now we need to copy tail data to our current element

                current->value = tail->value;

                delete tail;

            }
            else {
                // big value, inefficient copy

                *parentPointer = tail;

                if (tail == tailParent) {

                    tail->left = current->left;
                    // no need to update tail->right, this pointer is already pointing to the correct subtree
                    // because current->right == tail

                }
                else {

                    tailParent->left = tail->right;

                    tail->left = current->left;
                    tail->right = current->right;

                }

                delete current;

            }

        }
        else {

            BSTNode<T>* tailParent = current->left;
            BSTNode<T>* tail = current->left;

            // find all the successors

            while (tail->right != nullptr) {
                tailParent = tail;
                tail = tail->right;
            }

            // we have reached the end of the row

            if (sizeof(T) <= sizeof(void*) + sizeof(void*)) {
                // copy data

                if (tail == tailParent) {
                    // short row (the right child of current doesn't have a left subtree)
                    current->left = tail->left;
                }
                else {
                    tailParent->right = tail->left;
                }

                // now we need to copy tail data to our current element

                current->value = tail->value;

                delete tail;

            }
            else {
                // big value, inefficient copy

                *parentPointer = tail;

                if (tail == tailParent) {

                    tail->right = current->right;
                    // no need to update tail->right, this pointer is already pointing to the correct subtree
                    // because current->left == tail

                }
                else {

                    tailParent->right = tail->left;

                    tail->left = current->left;
                    tail->right = current->right;

                }

                delete current;

            }

        }

        rightSubtree = !rightSubtree;

        return true;

    }

};

#endif //BST_BST_HPP