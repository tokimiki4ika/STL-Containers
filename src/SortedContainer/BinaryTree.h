#ifndef STL_CONTAINERS_BINARYTREE_H_
#define STL_CONTAINERS_BINARYTREE_H_

#include  <iostream>

#include <memory_resource>
#include <functional>
#include <limits>

namespace s21 {
template <class Key, class Compare = std::less<Key>>
class BinaryTree {
    // pre-implementation =======================================================
protected:
    class Node;
public:
    class TreeIterator;
    class TreeConstIterator;

    // using ====================================================================
    using key_type = Key;
    using value_type = key_type;
    using reference = key_type&;
    using const_reference = const key_type&;

    using size_type = std::size_t;
    using node_type = Node;
    using pointer = node_type*;
    using iterator = TreeIterator;
    using const_iterator = const TreeIterator;

protected:
    iterator CreateIterator(pointer node) { // private
        return TreeIterator(node, _begin, _end, _size);
    }
public:
    BinaryTree() {
        _begin = _root = _end = new Node();
        _begin->_left = _end;
        _end->_right = _begin;
    }
    BinaryTree(const std::initializer_list<key_type>& items) {
        _begin = _root = _end = new Node();
        for (key_type item : items) {
            insert(item);
        }
    }
    ~BinaryTree() {
        delete _root;
    }

    void clear() {
        _begin->_left = _end->_right = nullptr;
        _end->_parent->_right = nullptr;
        delete _root;
        _begin = _root = _end;
        _size = 0;
    }

//    iterator lower_bound(const_reference key) {
//        return TreeIterator(begin()); // недоделка
//    }

    iterator insert(const_reference key) {
        pointer current_node = _root;
        if (current_node == _end) {
            current_node = _begin = _root = new Node(key);
            _begin->_right = _end;
            _end->_parent = _begin;
        } else {
            while (true) {
                if (Compare{}(key, current_node->_key)) {
                    if (current_node->_left != nullptr) {
                        current_node = current_node->_left;
                    } else {
                        current_node->_left = new Node(key, current_node);
                        if (current_node == _begin) {
                            _begin = current_node->_left;
                        }
                        current_node = current_node->_left;
                        break;
                    }
                } else if (current_node->_right != nullptr && current_node->_right != _end) {
                    current_node = current_node->_right;
                } else {
                    if (current_node->_right == _end) {
                        _end->_parent = current_node->_right = new Node(key, current_node);
                        current_node->_right->_right = _end;
                    } else {
                        current_node->_right = new Node(key, current_node);
                    }
                    current_node = current_node->_right;
                    break;
                }
            }
        }
        ++_size;
        return CreateIterator(current_node);
    }

    size_type size() noexcept {
        return _size;
    }

    iterator begin() {
        return CreateIterator(_begin);
    }

    iterator end() {
        return CreateIterator(_end);
    }

//    iterator cbegin() const noexcept {
//        return TreeIterator(_begin);
//    }
//
//    iterator cend() const noexcept {
//        return TreeIterator(_end);
//    }

    bool empty() {
        return _size == 0;
    }

    size_type max_size() {
        return std::numeric_limits<size_type>::max() / sizeof(key_type) / 2;
    }

protected:
    pointer _root{}, _begin{}, _end{};
    size_type _size{};
};

// node =====================================================================

template <class Key, class Compare>
class BinaryTree<Key, Compare>::Node {
public:
    Node() = default;
    explicit Node(key_type key) : _key(key) {}
    Node(key_type key, node_type *node) : _key(key), _parent(node) {}
    ~Node() { delete _left; delete _right; }
    key_type _key{};
    pointer _left{}, _right{}, _parent{};
};

// iterators ================================================================

template <class Key, class Compare>
class BinaryTree<Key, Compare>::TreeIterator {
public:
    TreeIterator() = delete;
    TreeIterator(pointer current, pointer &begin, pointer &end, size_type &size)
    : current_node(current), _begin(begin), _end(end), _size(size) {}
    TreeIterator(TreeIterator &other)
            : current_node(other.current_node), _begin(other._begin), _end(other._end), _size(other._size) {}
    TreeIterator(TreeIterator &&other)
            : current_node(other.current_node), _begin(other._begin), _end(other._end), _size(other._size) {}
    ~TreeIterator() = default;

    value_type operator*() {
        if (current_node == _end && std::is_arithmetic<value_type>{}) {
            return value_type{_size};
        }
        return current_node->_key;
    }

    value_type operator*() const {
        if (current_node == _end && std::is_arithmetic<value_type>{}) {
            return value_type{_size};
        }
        return current_node->_key;
    }

    iterator operator++() {
        if (current_node == _end) {
            current_node = _begin;
            return *this;
        }
        pointer last_position{};
        while (current_node->_right == nullptr || current_node->_right == last_position) {
            last_position = current_node;
            current_node = current_node->_parent;
        }
        current_node = current_node->_right;
        while (current_node->_left != nullptr) {
            current_node = current_node->_left;
        }
        return *this;
    }

    iterator operator--() {
        if (current_node == _begin) {
            current_node = _end;
            return *this;
        }
        if (current_node->_left == nullptr) {
            pointer last_position{};
            while (current_node->_left == last_position) {
                last_position = current_node;
                current_node = current_node->_parent;
            }
        } else {
            current_node = current_node->_left;
        }
        return *this;
    }

    bool operator!=(TreeIterator other) {
        return other.current_node != current_node;
    }

    bool operator==(TreeIterator other) {
        return other.current_node == current_node;
    }

protected:
    pointer current_node;
    const pointer &_end, &_begin;
    size_type &_size;
};

//template <typename Key, typename Compare>
//class BinaryTree<Key, Compare>::TreeConstIterator {
//
//};

} // namespace s21

#endif //STL_CONTAINERS_BINARYTREE_H_
