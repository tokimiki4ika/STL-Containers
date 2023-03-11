#ifndef STL_CONTAINERS_MULTISET_H_
#define STL_CONTAINERS_MULTISET_H_

#include "../BinaryTree.h"

namespace s21 {

template<class Key>
class multiset : public BinaryTree<Key> {
public:
    using key_type = Key;
    using value_type = Key;
    using reference = value_type&;
    using const_reference = const value_type&;

    using size_type = std::size_t;
    using node_type = class BinaryTree<Key>::Node;
    using pointer = node_type*;
    using iterator = class BinaryTree<Key>::TreeIterator;
    using const_iterator = class BinaryTree<Key>::TreeConstIterator;

    multiset() : BinaryTree<Key>() {}
    multiset(const std::initializer_list<key_type> &items) : BinaryTree<Key>(items) {}
    multiset(const multiset &ms) {

    }
    multiset(multiset &&ms) {
        *this = std::move(ms);
    }
    ~multiset() = default;

    multiset &operator=(multiset &&ms) {
        delete this->_root;
        this->_begin = ms._begin;
        this->_end = ms._end;
        this->_root = ms._root;
        this->_size = ms._size;
        ms._begin = ms._end = ms._root = new node_type();
        ms._size = 0;
    }

    void swap(multiset& other) {
        multiset tmp = std::move(other);
        other = std::move(*this);
        *this = std::move(tmp);
    }

    void merge(multiset& other) {
        for (value_type item : other) {
            insert(item);
        }
        other.clear();
    }

};

} // namespace s21

#endif //STL_CONTAINERS_MULTISET_H_
