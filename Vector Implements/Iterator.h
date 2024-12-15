#pragma once
#include <iterator>  

template<class T>
class Iterator
{
public:
    using iterator_category = std::random_access_iterator_tag; 
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    Iterator(T* p = nullptr) : _curr(p) {}

    Iterator& operator++()
    {
        ++_curr;
        return *this;
    }

    Iterator& operator--()
    {
        --_curr;
        return *this;
    }

    Iterator& operator+=(difference_type n)
    {
        _curr += n;
        return *this;
    }

    Iterator& operator-=(difference_type n)
    {
        _curr -= n;
        return *this;
    }

    Iterator operator+(difference_type n) const
    {
        return Iterator(_curr + n);
    }

    Iterator operator-(difference_type n) const
    {
        return Iterator(_curr - n);
    }

    difference_type operator-(const Iterator& other) const
    {
        return _curr - other._curr;
    }

    reference operator*() const
    {
        return *_curr;
    }

    pointer operator->() const
    {
        return _curr;
    }

    reference operator[](difference_type n) const
    {
        return *(_curr + n);
    }

    bool operator==(const Iterator& b) const
    {
        return _curr == b._curr;
    }

    bool operator!=(const Iterator& b) const
    {
        return _curr != b._curr;
    }

    bool operator<(const Iterator& b) const
    {
        return _curr < b._curr;
    }

    bool operator<=(const Iterator& b) const
    {
        return _curr <= b._curr;
    }

    bool operator>(const Iterator& b) const
    {
        return _curr > b._curr;
    }

    bool operator>=(const Iterator& b) const
    {
        return _curr >= b._curr;
    }

private:
    T* _curr;
};
