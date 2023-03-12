//
// Created by 12355 on 05.03.2023.
//

#include <iostream>
#include "BinaryTree.h"

int main() {
    s21::BinaryTree<int> a {2, 3, 4, 1};
    a.clear();
    std::cout << a.empty() << " " << a.max_size() << std::endl;
    a.insert(4);
    a.insert(6);
    a.insert(1);
    a.insert(0);
    a.insert(2);
    a.insert(3);
    a.insert(2);
    a.insert(2);
    a.insert(0);
    a.insert(0);
    a.insert(3);
    a.insert(3);
    auto b = a.equal_range(0);
    for (; b.first != b.second;) {
        if (b.first != b.second) {
            std::cout << "value: " << *b.first++ << std::endl;
        }
    }
    std::cout << "count 0: " << a.count(0) << std::endl;
    return 0;
}
