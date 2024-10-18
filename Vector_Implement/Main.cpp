#include "Vector.h"
#include <iostream>
#include <cstdlib> 

int main() {
    Vector<int> vec;

    int num;
    std::cout << "Number of elements: ";
    std::cin >> num;

    for (int i = 0; i < num; i++) {
        vec.push_back(i); 
    }

    for (Vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";  
    }
    std::cout << std::endl;

    return 0;
}
