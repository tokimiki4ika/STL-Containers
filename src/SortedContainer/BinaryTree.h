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
    using const_iterator = TreeConstIterator;

protected:
//    const_iterator CreateConstIterator(pointer node) {
//        return TreeConstIterator(node, _begin, _end, _size);
//    }

//    const_iterator CreateConstIterator(pointer node) const {
//        return TreeConstIterator(node, _begin, _end, _size);
//    }

    iterator CreateIterator(pointer node) {
        return TreeIterator(node, _begin, _end, _size);
    }

//    iterator CreateIterator(pointer node) const {
//        return TreeIterator(node, _begin, _end, _size);
//    }

public:
    BinaryTree() {
        _begin = _root = _end = new Node();
        _begin->_left = _end;
        _end->_right = _begin;
    }
    BinaryTree(const std::initializer_list<key_type> &items) {
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

    iterator insert(const_reference value) {
        pointer current_node = _root;
        if (_root == _end) {
            current_node = _begin = _root = new Node(value);
            _begin->_right = _end;
            _end->_parent = _begin;
        } else {
            while (true) {
                if (Compare{}(value, current_node->_value)) {
                    if (current_node->_left != nullptr) {
                        current_node = current_node->_left;
                    } else {
                        current_node->_left = new Node(value, current_node);
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
                        _end->_parent = current_node->_right = new Node(value, current_node);
                        current_node->_right->_right = _end;
                    } else {
                        current_node->_right = new Node(value, current_node);
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

    size_type size() const noexcept {
        return _size;
    }

    iterator begin() {
        return CreateIterator(_begin);
    }

    iterator begin() const {
        return CreateIterator(_begin);
    }

    iterator end() {
        return CreateIterator(_end);
    }

    iterator end() const {
        return CreateIterator(_end);
    }

    const_iterator cbegin() {
        return TreeConstIterator(_begin, _begin, _end, _size);
    }
//
//    const_iterator cbegin() const {
//        return CreateIterator(_begin);
//    }
//
    const_iterator cend() {
        return TreeConstIterator(_end, _begin, _end, _size);
    }
//
//    const_iterator cend() const {
//        return CreateIterator(_end);
//    }

    bool empty() {
        return _size == 0;
    }

    bool empty() const {
        return _size == 0;
    }

    size_type max_size() {
        return std::numeric_limits<size_type>::max() / sizeof(key_type) / 2;
    }

    size_type max_size() const {
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
    explicit Node(key_type value) : _value(value) {}
    Node(key_type value, node_type *node) : _value(value), _parent(node) {}
    ~Node() { delete _left; delete _right; }
    key_type _value{};
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
        if (current_node == _end/* && std::is_arithmetic<value_type>{}*/) {
            return value_type{_size};
        }
        return current_node->_value;
    }

    /*virtual */TreeIterator operator++() {
        if (current_node == _end) {
            current_node = _begin;
            return *this;
        }
        while (current_node->_right == nullptr) {
            pointer last_position = current_node;
            current_node = current_node->_parent;
            if (current_node->_right != last_position) {
                return *this;
            }
        }
        current_node = current_node->_right;
        while (current_node->_left != nullptr) {
            current_node = current_node->_left;
        }
        return *this;
    }

    /*virtual */TreeIterator operator++(int) {
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

    /*virtual */TreeIterator operator--() {
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

    /*virtual */TreeIterator operator--(int) {
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

    bool operator!=(TreeIterator other) const {
        return other.current_node != current_node;
    }

    bool operator==(TreeIterator other) {
        return other.current_node == current_node;
    }

    bool operator==(TreeIterator other) const {
        return other.current_node == current_node;
    }

protected:
    pointer current_node;
    const pointer &_end, &_begin;
    const size_type &_size;
};

template <class Key, class Compare> // пока не понятно что с этим делать
class BinaryTree<Key, Compare>::TreeConstIterator : public BinaryTree<Key, Compare>::TreeIterator {
public:
    TreeConstIterator() = delete;
    TreeConstIterator(pointer current, pointer &begin, pointer &end, size_type &size)
    : BinaryTree<Key, Compare>::TreeIterator(current, begin, end, size) {}
};

} // namespace s21

#endif //STL_CONTAINERS_BINARYTREE_H_
