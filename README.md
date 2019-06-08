# Optimized BST
This is my production-ready, optimized Binary Search Tree implementation.

**NOTE**: A BST is not a balanced binary search tree by definition . Although this implementation supports delete-trigger based balancing attempts, do not expect `O(log n)` search / delete complexity. BST's don't guarantee this.

# What's interesting about this implementation?
* Heavily optimized compared to many other implementations on the internet, production ready.
* No recursion (except tree traversal)
* Only one function call for search / delete / insert operations.
* Although a BST is by definition not balanced, it attempts to balance itself upon deletions of nodes with both children present (This feature can easily be disabled if you don't need it).
* Unique / non unique element insertion.
* 2 delete techniques supported:
  1. With node payload copying but less pointer operations (efficient when node's payload is less than `2 * sizeof(void*)`).
  2. With only pointer rearrangement (efficient when the node's payload is large).
* Implemented using C++ templates.
* Debug printing method with indented, tree-like visual output.
* Lightweight and single-header.