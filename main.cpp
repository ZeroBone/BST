#include <iostream>
#include <fstream>

#include "bst.hpp"

int main() {

    auto tree = BST<int>();

    std::ifstream file("tree.txt");

    {
        int v;
        while (file >> v) {
            tree.insertUnique(v);
        }
    }

    file.close();

    std::cout << "Tree:" << std::endl;
    tree.debugPrint();

    while (true) {

        int key;
        std::cout << "Enter key to delete or 0 to exit:" << std::endl;
        std::cin >> key;

        if (key == 0) {
            break;
        }

        if (tree.remove(key)) {
            std::cout << "Element successfully deleted." << std::endl;
        }
        else {
            std::cout << "Element not found." << std::endl;
        }

        std::cout << "New tree:" << std::endl;
        tree.debugPrint();

    }

    system("pause");

    return 0;

}