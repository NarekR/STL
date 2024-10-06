#include <iostream>

template<typename T>
struct remove_reference {
    using type = T;
};

template<typename T>
struct remove_reference<T&> {
    using type = T;
};

template<typename T>
struct remove_reference<T&&> {
    using type = T;
};


template<typename T>
typename remove_reference<T>::type&& move(T&& arg) {
    return static_cast<typename remove_reference<T>::type&&>(arg);
}

template<typename T>
T&& forward(typename remove_reference<T>::type& arg) {
    return static_cast<T&&>(arg);
}

template<typename T>
T&& forward(typename remove_reference<T>::type&& arg) {
    static_assert(!std::is_lvalue_reference<T>::value, "Forwarding an rvalue as an lvalue!");
    return static_cast<T&&>(arg);
}




