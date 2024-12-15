#include "Vector.h"
#include <iostream>
#include <cstdlib> 
#include <thread>
#include <cassert>


template <typename Iterator, typename Compare>
void merge(Iterator left, Iterator mid, Iterator right, Compare comp) {
    using ValueType = typename std::iterator_traits<Iterator>::value_type;

    Vector<int> temp;
    Iterator i = left;
    Iterator j = mid + 1;

    while (i <= mid && j <= right) {
        if (comp(*i, *j)) {
            temp.push_back(*i);
            ++i;
        }
        else {
            temp.push_back(*j);
            ++j;
        }
    }

    while (i <= mid) {
        temp.push_back(*i);
        ++i;
    }

    while (j <= right) {
        temp.push_back(*j);
        ++j;
    }

    Iterator k = left;
    for (size_t idx = 0; idx < temp.size(); ++idx) {
        *k = temp[idx];
        ++k;
    }
}


template <typename Iterator, typename Compare>
void merge_sort_single_thread(Iterator first, Iterator last, Compare comp) {
    if (std::distance(first, last) <= 1) return;

    Iterator mid = first + std::distance(first, last) / 2 - 1;
    merge_sort_single_thread(first, mid + 1, comp);
    merge_sort_single_thread(mid + 1, last, comp);
    merge(first, mid, last - 1, comp);
}

template <typename Iterator, typename Compare>
void merge_sort(Iterator first, Iterator last, Compare comp, int threads = std::thread::hardware_concurrency()) {
    static_assert(
        std::is_same_v<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>,
        "merge_sort requires Random Access Iterators."
        );

    if (std::distance(first, last) <= 1) return;

    if (threads <= 1) {
        merge_sort_single_thread(first, last, comp);
        return;
    }

    Iterator mid = first + std::distance(first, last) / 2 - 1;

    std::thread left_thread([=, &comp]() {
        merge_sort(first, mid + 1, comp, threads / 2);
        });

    std::thread right_thread([=, &comp]() {
        merge_sort(mid + 1, last, comp, threads / 2);
        });

    left_thread.join();
    right_thread.join();

    merge(first, mid, last - 1, comp);
}


int main() {
    Vector<int> vec1;

    int num;
    int threads;
    std::cout << "Number of elements: ";
    std::cin >> num;
    std::cout << "Number of Threads: ";
    std::cin >> threads;

    for (int i = 0; i < num; i++) {
        vec1.push_back(rand());
    }

    for (auto it = vec1.begin(); it != vec1.end(); ++it) {
        std::cout << *it << " ";  
    }

    std::cout <<"\n==================================================================================" << std::endl;
    auto func = [](const int& a, const int& b) { return a < b; };
    merge_sort(vec1.begin(), vec1.end(), func, threads);

    for (auto it = vec1.begin(); it != vec1.end(); ++it) {
        std::cout << *it << " ";
    }

    return 0;
}
