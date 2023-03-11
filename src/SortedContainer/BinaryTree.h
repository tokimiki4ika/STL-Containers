#ifndef STL_CONTAINERS_BINARYTREE_H_
#define STL_CONTAINERS_BINARYTREE_H_

#include <limits>
#include <initializer_list>
#include <functional>
#include <stdexcept>

namespace s21 {
template <class Key>
class BinaryTree {
    // pre-implementation =======================================================
protected:
    class Node;
    class TreeIterator;
    class TreeConstIterator;
public:
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

protected:
    using Compare = std::less<Key>;

    const_iterator CreateIterator(pointer node) noexcept {
        return TreeConstIterator(node, _begin, _end, _size);
    }

    pointer FindFirstEqualOrNextPointer(const_reference value) const noexcept {
        pointer current_node = _root;
        while (current_node != _end) {
            if (Compare{}(value, current_node->_value)) {
                if (current_node->_left != nullptr) {
                    current_node = current_node->_left;
                } else {
                    break;
                }
            } else if (value == current_node->_value) {
                return current_node;
            } else if (current_node->_right != nullptr && current_node->_right != _end) {
                current_node = current_node->_right;
            } else {
                break;
            }
        }
        return current_node;
    }

public:

    void clear() {
        _end->_parent = _end->_parent->_right = nullptr;
        delete _root;
        _begin = _root = _end;
        _size = 0;
    }

   virtual bool contains(const key_type& key) {
        pointer current = FindFirstEqualOrNextPointer(key);
        return key == current->_value;
   }

   void erase(iterator pos) {}

   virtual size_type count(const key_type& key) {
       iterator iter = find(key);
       size_type size{};
       while (*iter == key && iter != end()) {
           ++iter, ++size;
       }
       return size;
   }

   virtual iterator find(const key_type& key) {
       pointer current = FindFirstEqualOrNextPointer(key);
       if (current->_value == key) {
           while (current->_left) {
               if (current->_left->_value == key) {
                   current = current->_left;
               } else {
                   break;
               }
           }
           return CreateIterator(current);
       }
       return end();
   }

   virtual iterator lower_bound(const key_type& key) {
       pointer current = FindFirstEqualOrNextPointer(key);
       while (current->_left) {
           if (current->_left->_value == key) {
               current = current->_left;
           } else {
               break;
           }
       }
       return CreateIterator(current);
   }

   virtual iterator upper_bound(const key_type& key) {
       pointer current = FindFirstEqualOrNextPointer(key);
       iterator iter = CreateIterator(current);
       while (iter != end()) {
           if (*iter == key) {
               ++iter;
           } else {
               break;
           }
       }
       return iter;
   }

   std::pair<iterator,iterator> equal_range(const key_type& key) {
       return std::pair<iterator, iterator>{lower_bound(key), upper_bound(key)};
   }

   virtual iterator insert(const_reference value) {
       if (max_size() == _size) {
           throw std::overflow_error("Can't insert new element, because size will over max_size");
       } // пока не разобрался, но думаю нужно заменить
       pointer current_node;
       if (_root == _end) {
           current_node = _begin = _root = new Node(value);
           _begin->_right = _end;
           _end->_parent = _begin;
       } else {
           current_node = FindFirstEqualOrNextPointer(value);
           if (Compare{}(value, current_node->_value)) {
               current_node->_left = new Node(value, current_node);
               current_node = current_node->_left;
               if (current_node->_parent == _begin) {
                   _begin = current_node;
               }
           } else if (value == current_node->_value) {
               pointer buff = current_node->_left;
               current_node->_left = new Node(value, current_node);
               current_node->_left->_left = buff;
               if (buff) {
                   buff->_parent = current_node->_left;
               }
               if (current_node == _begin) {
                   _begin = current_node->_left;
               }
               current_node->_left->_left = buff;
           } else {
               if (current_node->_right == _end) {
                   _end->_parent = current_node->_right = new Node(value, current_node);
                   current_node->_right->_right = _end;
               } else {
                   current_node->_right = new Node(value, current_node);
               }
               current_node = current_node->_right;
           }
       }
       ++_size;
       return CreateIterator(current_node);
   }

   [[nodiscard]] size_type size() const noexcept {
       return _size;
   }

   [[nodiscard]] iterator begin() noexcept {
       return CreateIterator(_begin);
   }

   [[nodiscard]] iterator end() noexcept {
       return CreateIterator(_end);
   }

   [[nodiscard]] const_iterator cbegin() noexcept {
       return CreateIterator(_begin);
   }

   [[nodiscard]] const_iterator cend() noexcept {
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

template <class Key>
class BinaryTree<Key>::Node {
public:
    Node() = default;
    explicit Node(key_type value) : _value(value) {}
    Node(key_type value, node_type *node) : _value(value), _parent(node) {}
    ~Node() { delete _left; delete _right; }
    key_type _value{};
    pointer _left{}, _right{}, _parent{};
};

// iterators ================================================================

template <class Key>
class BinaryTree<Key>::TreeIterator {
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
        pointer last_position{};
        while (current_node->_right == nullptr || last_position == current_node->_right) {
            last_position = current_node;
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
        iterator it = *this;
        ++*this;
        return it;
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
        iterator it = *this;
        --*this;
        return it;
    }

    [[nodiscard]] bool operator!=(const TreeIterator other) const noexcept {
        return other.current_node != current_node;
    }

    [[nodiscard]] bool operator==(const TreeIterator other) const noexcept {
        return other.current_node == current_node;
    }

protected:
    pointer current_node{}, &_begin{}, &_end{};
    size_type &_size;
};

template <class Key>
class BinaryTree<Key>::TreeConstIterator : public BinaryTree<Key>::TreeIterator {
public:
    TreeConstIterator() = delete;
    TreeConstIterator(pointer current, pointer &begin, pointer &end, size_type &size) noexcept
    : BinaryTree<Key>::TreeIterator(current, begin, end, size) {}
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
