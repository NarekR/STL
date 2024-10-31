#include <iostream>
//#include <utility>

template <typename T>
class UniquePtr {
private:
    T* ptr;

public:
    UniquePtr(T* ptr = nullptr) : ptr(ptr) {}
    ~UniquePtr() {
        delete ptr;
    }
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    T* get() const { return ptr; }

    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }
    void reset(T* newPtr = nullptr) {
        delete ptr;
        ptr = newPtr;
    }
};

template <typename T>
class SharedPtr {
private:
    T* ptr;
    int* count;

public:
    SharedPtr(T* ptr = nullptr) : ptr(ptr), count(new int(1)) {}

    SharedPtr(const SharedPtr& other) : ptr(other.ptr), count(other.count) {
        if (ptr) {
            (*count)++;
        }
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this == &other) return *this;

        release();

        ptr = other.ptr;
        count = other.count;
        if (ptr) {
            (*count)++;
        }
        return *this;
    }

    ~SharedPtr() {
        release();
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    int use_count() const { return (ptr) ? *count : 0; }

private:
    void release() {
        if (ptr && --(*count) == 0) {
            delete ptr;
            delete count;
            ptr = nullptr;
            count = nullptr;
        }
    }
};


template <typename T>
class WeakPtr {
private:
    T* ptr;
    int* count;

public:
    WeakPtr() : ptr(nullptr), count(nullptr) {}

    WeakPtr(const SharedPtr<T>& sharedPtr) : ptr(sharedPtr.ptr), count(sharedPtr.count) {}

    bool expired() const { return !count || *count == 0; }

    SharedPtr<T> lock() const {
        if (expired()) return SharedPtr<T>();
        return SharedPtr<T>(*this);
    }
};


int main() {
    {
        UniquePtr<int> uptr(new int(100));
        std::cout << "UniquePtr value: " << *uptr << "\n";

        UniquePtr<int> uptr2 = std::move(uptr);
        if (!uptr.get()) {
            std::cout << "uptr is now empty after move\n";
        }
        std::cout << "UniquePtr2 value: " << *uptr2 << "\n";
    } 

    std::cout << "-----\n";
    {
        SharedPtr<int> sptr1(new int(42));
        std::cout << "sptr1 address is " << &sptr1 << "\tValue = " << *sptr1 << "\n";
        std::cout << "sptr1 use count: " << sptr1.use_count() << "\n";

        SharedPtr<int> sptr2 = sptr1;
        std::cout << "sptr2 address is " << &sptr2 << "\tValue = " << *sptr2 << "\n";
        std::cout << "sptr1 use count after sptr2 copy: " << sptr1.use_count() << "\n";
        std::cout << "sptr2 use count: " << sptr2.use_count() << "\n";

        {
            SharedPtr<int> sptr3 = sptr1;
            std::cout << "sptr3 created. sptr1 use count: " << sptr1.use_count() << "\n";
        }
        std::cout << "sptr1 use count after sptr3 is destroyed: " << sptr1.use_count() << "\n";
        sptr2 = new int(50);
        std::cout << "sptr1 use count after sptr2 switch: " << sptr1.use_count() << "\n";
    } 

    std::cout << "-----\n";
    return 0;
}

