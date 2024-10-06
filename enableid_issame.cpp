#include <iostream>

template<bool Cond, typename T = void>
struct enable_if {};

template<typename T>
struct enable_if<true, T> {
    using type = T;
};

template<bool Condition, typename T = void>
using enable_if_t = typename enable_if<Condition, T>::type;


template<typename T>
typename enable_if_t<std::is_integral<T>::value, T> add(T a, T b) {
    return a + b;
}

template<typename T>
typename enable_if_t<!std::is_integral<T>::value, T> add(T a, T b) {
    std::cout << "Addition for non-integral types!";
    return a + b;
}

template<typename T, typename U>
struct is_same {
    static const bool value = false;
};

template<typename T>
struct is_same<T, T> {
    static const bool value = true;
};


template<typename T, typename U>
constexpr bool is_same_v = is_same<T, U>::value;


int main() {
    std::cout << add(3, 4) << std::endl;       
    std::cout << add(3.5, 4.5) << std::endl;
    std::cout << "<int - int>: " << is_same<int, int>::value << std::endl;   
    std::cout << "<int - float>: " << is_same<int, float>::value << std::endl; 
}
