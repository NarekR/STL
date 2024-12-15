#include <iostream>
#include <list>

template <typename T>
std::list<T> merge(const std::list<T>& left, const std::list<T>& right) {
    std::list<T> result;
    auto it_left = left.begin();
    auto it_right = right.begin();

    while (it_left != left.end() && it_right != right.end()) {
        if (*it_left <= *it_right) {
            result.push_back(*it_left);
            ++it_left;
        }
        else {
            result.push_back(*it_right);
            ++it_right;
        }
    }

    result.insert(result.end(), it_left, left.end());
    result.insert(result.end(), it_right, right.end());

    return result;
}

template <typename T>
void split(std::list<T>& input, std::list<T>& left, std::list<T>& right) {
    auto mid = input.begin();
    std::advance(mid, input.size() / 2); 

    left.splice(left.begin(), input, input.begin(), mid);
    right.splice(right.begin(), input, mid, input.end());
}

template <typename T>
void mergeSort(std::list<T>& list) {
    if (list.size() <= 1) {
        return;
    }

    std::list<T> left, right;
    split(list, left, right);  

    mergeSort(left);  
    mergeSort(right); 
    list = merge(left, right);
}

int main() {
    
    std::list<int> numbers;
    int num;
    std::cout << "Number of elements: ";
    std::cin >> num;

    for (size_t i = 0; i < num; i++)
    {
        numbers.push_back(rand());

    }
    std::cout << "begin: \n";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    mergeSort(numbers);

    std::cout << "after: \n";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
