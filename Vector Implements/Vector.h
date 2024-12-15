#pragma once
#include <mutex>
#include <shared_mutex>
#include "Iterator.h"
#include "Allocator.h"

template<class T, class Alloc = Allocator<T>>
class Vector {
public:
    using iterator = Iterator<T>;
    using allocator_type = Alloc;

    Vector();
    Vector(int s);

    Vector(const Vector& arg);
    Vector& operator=(const Vector& arg);
    T& operator[](size_t index) {
        return _data[index];
    }

    const T& operator[](size_t index) const {
        return _data[index];
    }

    ~Vector();

    iterator begin() const;
    iterator end() const;

    bool empty() const;
    size_t capacity() const;
    void reserve(int newmalloc);
    void resize(int newsize, T val = T());
    size_t size() const;

    void clear();
    void push_back(const T& d);
    void pop_back();

private:
    size_t _size;
    size_t _capacity;
    T* _data;
    Alloc _alloc;  
    mutable std::shared_mutex _mutex;
};

template <class T, class Alloc>
Vector<T, Alloc>::Vector()
    : _size(0), _capacity(0), _data(nullptr), _alloc() {}

template <class T, class Alloc>
Vector<T, Alloc>::Vector(int s)
    : _size(s), _capacity(s), _alloc() {
    _data = _alloc.allocate(s);
    for (int i = 0; i < _size; ++i) {
        _alloc.construct(&_data[i]);
    }
}

template <class T, class Alloc>
Vector<T, Alloc>::~Vector() {
    for (int i = 0; i < _size; ++i) {
        _alloc.destroy(&_data[i]);
    }
    _alloc.deallocate(_data, _capacity);
}

template<class T, class Alloc>
Vector<T, Alloc>::Vector(const Vector& arg)
{
    std::unique_lock<std::shared_mutex> lock(arg._mutex);
    _size = arg._size;
    _capacity = arg._capacity;
    _data = new T[arg._size];
    for (int index = 0; index < arg._size; ++index)
        _data[index] = arg._data[index];
}

template<class T, class Alloc>
Vector<T, Alloc>& Vector<T, Alloc>::operator=(const Vector<T, Alloc>& a)
{
    if (this == &a) return *this;

    std::unique_lock<std::shared_mutex> lock_a(a._mutex);  
    std::unique_lock<std::shared_mutex> lock_this(_mutex);

    if (a._size <= _capacity)
    {
        for (int index = 0; index < a._size; ++index)
        {
            _data[index] = a._data[index];
        }
        _size = a._size;
        return *this;
    }

    T* p = new T[a._size];
    for (int index = 0; index < a._size; ++index)
        p[index] = a._data[index];

    delete[] _data;
    _data = p;
    _size = a._size;
    _capacity = a._size;
    return *this;
}


template<class T, class Alloc>
Iterator<T> Vector<T, Alloc>::begin() const {
    std::shared_lock<std::shared_mutex> lock(_mutex);
    return Iterator<T>(_data);
}

template<class T, class Alloc>
Iterator<T> Vector<T, Alloc>::end() const {
    std::shared_lock<std::shared_mutex> lock(_mutex);
    return Iterator<T>(_data + _size);
}




template<class T, class Alloc>
bool Vector<T, Alloc>::empty() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);  
    return (_size == 0);
}

template<class T, class Alloc>
size_t Vector<T, Alloc>::capacity() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);  
    return _capacity;
}

template<class T, class Alloc>
void Vector<T, Alloc>::reserve(int newalloc)
{
    //std::unique_lock<std::shared_mutex> lock(_mutex); 
    if (newalloc <= _capacity) return;

    T* p = new T[newalloc];
    for (int i = 0; i < _size; ++i)
        p[i] = _data[i];

    delete[] _data;
    _data = p;
    _capacity = newalloc;
}

template<class T, class Alloc>
void Vector<T, Alloc>::resize(int newsize, T val)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);  
    reserve(newsize);
    for (int index = _size; index < newsize; ++index)
        _data[index] = T();
    _size = newsize;
}

template<class T, class Alloc>
size_t Vector<T, Alloc>::size() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex); 
    return _size;
}


template<class T, class Alloc>
void Vector<T, Alloc>::push_back(const T& d)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    if (_capacity == 0)
        reserve(8);
    else if (_size == _capacity)
        reserve(2 * _capacity);

    _data[_size] = d;
    ++_size;
}

template<class T, class Alloc>
void Vector<T, Alloc>::pop_back()
{
    std::unique_lock<std::shared_mutex> lock(_mutex);  
    if (_size > 0)
        --_size;
}
