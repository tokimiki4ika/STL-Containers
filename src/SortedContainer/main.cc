//
// Created by 12355 on 05.03.2023.
//

#include <iostream>
#include <set>
#include "BinaryTree.h"

int main() {
    s21::BinaryTree<int, std::greater<>> a {2, 3, 4, 1};
//    std::multiset<int, std::greater<>> a {2, 3, 4, 1};
    a.clear();
    std::cout << a.empty() << " " << a.max_size() << std::endl;
    a.insert(4);
    a.insert(6);
    a.insert(1);
    a.insert(0);
    a.insert(2);
    a.insert(3);
    a.insert(5);
    a.insert(5);
    a.insert(5);
    a.insert(5);
    a.insert(7);
    auto it = a.lower_bound(2), end = a.upper_bound(6);
//    auto it = a.lower_bound(1), end = a.upper_bound(6);
    for (; it != end;) {
        std::cout << "value: " << *it << std::endl;
        --it;
    }
//    std::cout << "size: " << *end << std::endl;
//    ++it;
//    for (; it != end;) {
//        std::cout << "value2: " << *it << std::endl;
//        ++it;
//    }
    return 0;
}

//int main() {
//    s21::BinaryTree<int, std::greater<>> a {2, 3, 4, 1};
////    std::multiset<int, std::greater<>> a {2, 3, 4, 1};
//    a.clear();
//    std::cout << a.empty() << " " << a.max_size() << std::endl;
//    a.insert(4);
//    a.insert(6);
//    a.insert(1);
//    a.insert(0);
//    a.insert(2);
//    a.insert(3);
//    a.insert(5);
//    a.insert(7);
//    auto it = a.lower_bound(1), end = a.upper_bound(6);
//    for (; it != end;) {
//        std::cout << "value: " << *it << std::endl;
//        --it;
//    }
////    std::cout << "size: " << *end << std::endl;
////    ++it;
////    for (; it != end;) {
////        std::cout << "value2: " << *it << std::endl;
////        ++it;
////    }
//    return 0;
//}

//int main() {
////    s21::BinaryTree<int> a {1, 2, 3, 4, 1};
//    std::set<int> a {1, 2, 3, 4, 1};
////    a.clear();
//    auto it = a.begin(), end = a.end();
//    a.clear();
//    a.insert(-2);
//    a.insert(3);
//    a.insert(1);
////    auto it2 = a.begin();
////    it = it2;
//    for (; it != end;) {
//        std::cout << "value: " << *it << std::endl;
//        ++it;
//    }
//    std::cout << "size " << *end;
//    return 0;
//}
