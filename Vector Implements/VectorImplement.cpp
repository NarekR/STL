#include <iostream>
#include <cstdlib> 
#include <cstring> 
#include<ctime>
#include<vector>
template<typename T>
class Allocator {
public:
    T* allocate(size_t size) {
        return static_cast<T*>(malloc(size * sizeof(T)));
    }

    T* reallocate(T* ptr, size_t new_size) {
        return static_cast<T*>(realloc(ptr, new_size * sizeof(T)));
    }

    void deallocate(T* ptr) {
        free(ptr);
    }
};

template<typename T, typename Alloc = Allocator<T>>
class Vector {
public:
    class Iterator {
    public:
        Iterator(T* ptr) : m_ptr(ptr) {}

        T& operator*() const { return *m_ptr; }

        Iterator& operator++() {
            m_ptr++;
            return *this;
        }

        Iterator& operator--() {
            m_ptr--;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return m_ptr != other.m_ptr;
        }

    private:
        T* m_ptr;
    };

    Vector() : m_data(nullptr), m_size(0), m_capacity(0) {}

    ~Vector() {
        clear();
    }

    void push_back(const T& value) {
        if (m_size >= m_capacity) {
            resize_capacity(m_capacity == 0 ? 1 : m_capacity * 2);
        }
        m_data[m_size++] = value;
    }

    T& operator[](size_t index) {
        return m_data[index];
    }

    const T& operator[](size_t index) const {
        return m_data[index];
    }

    size_t size() const {
        return m_size;
    }

    Iterator begin() {
        return Iterator(m_data);
    }

    Iterator end() {
        return Iterator(m_data + m_size);
    }

    void clear() {
        if (m_data) {
            alloc.deallocate(m_data);
        }
        m_data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }

private:
    T* m_data;     
    size_t m_size;  
    size_t m_capacity; 
    Alloc alloc;    

    void resize_capacity(size_t new_capacity) {
        T* new_data = alloc.allocate(new_capacity);
        if (m_data) {
            std::memcpy(new_data, m_data, m_size * sizeof(T));
            alloc.deallocate(m_data);
        }
        m_data = new_data;
        m_capacity = new_capacity;
    }
};

template <typename T>
void merge(Vector<T>& vec, size_t left, size_t mid, size_t right) {
    size_t i = left;
    size_t j = mid + 1;

    Vector<T> temp;
    while (i <= mid && j <= right) {
        if (vec[i] <= vec[j]) {
            temp.push_back(vec[i]);
            i++;
        }
        else {
            temp.push_back(vec[j]);
            j++;
        }
    }

    while (i <= mid) {
        temp.push_back(vec[i]);
        i++;
    }

    while (j <= right) {
        temp.push_back(vec[j]);
        j++;
    }

    for (size_t k = 0; k < temp.size(); ++k) {
        vec[left + k] = temp[k];
    }
}

template <typename T>
void merge_sort(Vector<T>& vec, size_t left, size_t right) {
    if (left >= right) {
        return; 
    }

    size_t mid = left + (right - left) / 2;
    merge_sort(vec, left, mid);
    merge_sort(vec, mid + 1, right);
    merge(vec, left, mid, right);
}

//int main() {
//    Vector<int> vec;
//    int num;
//    srand(time(NULL));
//    std::cout << "number of elelments in vector: ";
//    std::cin >> num;
//
//
//    for (int i = 0; i < num; i++) {
//        vec.push_back(rand());
//    }
//    
//    for (auto it = vec.begin(); it != vec.end(); ++it) {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    //merge_sort(vec, 0, vec.size()-1);
//    //for (auto it = vec.begin(); it != vec.end(); ++it) {
//    //    std::cout << *it << " ";
//    //}
//    return 0;
//}
