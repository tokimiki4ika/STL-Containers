#ifndef STL_CONTAINERS_BINARYTREE_H_
#define STL_CONTAINERS_BINARYTREE_H_

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
    using value_type = Key;
    using reference = value_type&;
    using const_reference = const value_type&;

    using size_type = std::size_t;
    using node_type = Node;
    using pointer = node_type*;
    using iterator = TreeIterator;
    using const_iterator = TreeConstIterator;

protected:
    const_iterator CreateIterator(pointer node) noexcept {
        return TreeConstIterator(node, _begin, _end, _size);
    }

public:
    BinaryTree() {
        _begin = _root = _end = new Node();
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
        _end->_parent->_right = nullptr;
        delete _root;
        _begin = _root = _end;
        _size = 0;
    }

    iterator insert(const_reference value) {
        if (max_size() == _size) {
            throw std::overflow_error("Can't insert new element, because size will over max_size");
        }
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

    [[nodiscard]] size_type size() const noexcept {
        return _size;
    }

    [[nodiscard]] iterator begin() {
        return CreateIterator(_begin);
    }

    [[nodiscard]] iterator end() {
        return CreateIterator(_end);
    }

    [[nodiscard]] const_iterator cbegin() {
        return CreateIterator(_begin);
    }

    [[nodiscard]] const_iterator cend() {
        return CreateIterator(_end);
    }

    [[nodiscard]] bool empty() const noexcept {
        return _size == 0;
    }

    [[nodiscard]] size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(node_type) / 2;
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
    TreeIterator(pointer current, pointer &begin, pointer &end, size_type &size) noexcept
    : current_node(current), _begin(begin), _end(end), _size(size) {}
    TreeIterator(TreeIterator &other) noexcept
    : current_node(other.current_node), _begin(other._begin), _end(other._end), _size(other._size) {}
    TreeIterator(TreeIterator &&other)  noexcept
    : current_node(other.current_node), _begin(other._begin), _end(other._end), _size(other._size) {}
    ~TreeIterator() = default;

    [[nodiscard]] value_type operator*() const {
        if (current_node == _end/* && std::is_arithmetic<value_type>{}*/) {
            return value_type(_size);
        }
        return current_node->_value;
    }

    TreeIterator operator++() {
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

    TreeIterator operator++(int) {
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

    TreeIterator operator--() {
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

    TreeIterator operator--(int) {
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

    [[nodiscard]] bool operator!=(TreeIterator other) const noexcept {
        return other.current_node != current_node;
    }

    [[nodiscard]] bool operator==(TreeIterator other) const noexcept {
        return other.current_node == current_node;
    }

protected:
    pointer current_node{}, &_begin{}, &_end{};
    size_type &_size;
};

template <class Key, class Compare>
class BinaryTree<Key, Compare>::TreeConstIterator : public BinaryTree<Key, Compare>::TreeIterator {
public:
    TreeConstIterator() = delete;
    TreeConstIterator(pointer current, pointer &begin, pointer &end, size_type &size) noexcept
    : BinaryTree<Key, Compare>::TreeIterator(current, begin, end, size) {}
    TreeConstIterator(TreeConstIterator &other) noexcept {
        this->current_node = other.current_node;
        this->_begin = other._begin;
        this->_end = other._end;
        this->_size = other._size;
    }
    TreeConstIterator(TreeConstIterator &&other) noexcept {
        this->current_node = other.current_node;
        this->_begin = other._begin;
        this->_end = other._end;
        this->_size = other._size;
    }
    ~TreeConstIterator() = default;
};

} // namespace s21

#endif //STL_CONTAINERS_BINARYTREE_H_
