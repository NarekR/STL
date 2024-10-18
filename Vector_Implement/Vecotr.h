#pragma once

#include <mutex>
#include <shared_mutex>

template<class T> class Vector
{
public:

    Vector();
    Vector(int s);

    Vector(const Vector& arg);
    Vector<T>& operator=(const Vector<T>& arg);

    ~Vector();

    class iterator;

    
    const iterator begin() const;
    const iterator end() const;

    bool empty() const;
    size_t capacity() const;
    void reserve(int newmalloc);
    void resize(int newsize, T val = T());
    size_t size() const;

    void clear();
    void push_back(const T& d);
    void pop_back(); 

private:
    size_t  _size;
    T* _data;
    size_t  _capacity;
    mutable std::shared_mutex _mutex; 
};


template<class T> class Vector<T>::iterator
{
public:
    iterator(T* p) :_curr(p)
    {}

    iterator& operator++()
    {
        _curr++;
        return *this;
    }

    iterator& operator--()
    {
        _curr--;
        return *this;
    }

    T& operator*()
    {
        return *_curr;
    }

    bool operator==(const iterator& b) const
    {
        return _curr == b._curr;
    }

    bool operator!=(const iterator& b) const
    {
        return _curr != b._curr;
    }

private:
    T* _curr;
};


template<class T>
Vector<T>::Vector()
    :_size(0), _data(0), _capacity(0)
{}

template<class T>
Vector<T>::Vector(int s)
    : _size(s), _data(new T[s]), _capacity(s)
{
    for (int index = 0; index < _size; ++index)
        _data[index] = T();
}

template<class T>
Vector<T>::Vector(const Vector& arg)
{
    std::unique_lock<std::shared_mutex> lock(arg._mutex);
    _size = arg._size;
    _capacity = arg._capacity;
    _data = new T[arg._size];
    for (int index = 0; index < arg._size; ++index)
        _data[index] = arg._data[index];
}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& a)
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

template<class T>
Vector<T>::~Vector()
{
    delete[] _data;
}


template<class T>
typename const Vector<T>::iterator Vector<T>::begin() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex); 
    return Vector<T>::iterator(&_data[0]);
}

template<class T>
typename const Vector<T>::iterator Vector<T>::end() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);  
    return Vector<T>::iterator(&_data[_size]);
}


template<class T>
bool Vector<T>::empty() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);  
    return (_size == 0);
}

template<class T>
size_t Vector<T>::capacity() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);  
    return _capacity;
}

template<class T>
void Vector<T>::reserve(int newalloc)
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

template<class T>
void Vector<T>::resize(int newsize, T val)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);  
    reserve(newsize);
    for (int index = _size; index < newsize; ++index)
        _data[index] = T();
    _size = newsize;
}

template<class T>
size_t Vector<T>::size() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex); 
    return _size;
}


template<class T>
void Vector<T>::push_back(const T& d)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    if (_capacity == 0)
        reserve(8);
    else if (_size == _capacity)
        reserve(2 * _capacity);

    _data[_size] = d;
    ++_size;
}

template<class T>
void Vector<T>::pop_back()
{
    std::unique_lock<std::shared_mutex> lock(_mutex);  
    if (_size > 0)
        --_size;
}
