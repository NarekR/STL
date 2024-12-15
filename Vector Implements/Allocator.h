#pragma once
#include <cstddef>  
#include <memory>   
#include <new>  
#include <limits>

template <typename T>
class Allocator {
public:
    using value_type = T;

    Allocator() noexcept = default;
    template <typename U>
    Allocator(const Allocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        if (n > max_size()) {
            throw std::bad_alloc();
        }
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t ) noexcept {
        ::operator delete(p);
    }
    
    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        ::new ((void*)p) U(std::forward<Args>(args)...);
    }

    
    template <typename U>
    void destroy(U* p) {
        p->~U();
    }

    std::size_t max_size() const noexcept {
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

    template <typename U>
    struct rebind {
        using other = Allocator<U>;
    };
};


template <typename T, typename U>
bool operator==(const Allocator<T>&, const Allocator<U>&) noexcept {
    return true;
}

template <typename T, typename U>
bool operator!=(const Allocator<T>&, const Allocator<U>&) noexcept {
    return false;
}
