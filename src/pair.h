#ifndef STL_CONTAINERS_PAIR_H_
#define STL_CONTAINERS_PAIR_H_

namespace s21 {
    template <class T1, class T2>
    class pair {
    public:
        pair() = delete;
        pair(T1 first_value, T2 second_value) : first(first_value), second(second_value) {}
        ~pair() = default;
        T1 first{};
        T2 second{};
    };

} // namespace s21

#endif //STL_CONTAINERS_PAIR_H_
