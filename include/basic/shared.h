#pragma once

#include <memory>
namespace dark {

namespace detail {

struct shared_counter {
    /**
     * Get the counter of the shared object.
     */
    template<typename _Tp>
    constexpr auto &operator()(_Tp &val) noexcept { return val.counter; }
};

struct shared_default {
    /**
     * Create a default dummy object.
     */
    template<typename _Tp>
    constexpr operator _Tp() { return _Tp(); }
};

} // namespace detail

template <
    typename _Tp,
    typename _Deleter = std::default_delete<_Tp>,
    typename _Counter = detail::shared_counter,
    typename _Default = detail::shared_default
>
struct shared_ptr {
private:
    auto &_M_counter() {
        return _Counter()(*this->_M_pointer);
    }

    void _M_deleter() {
        return _Deleter()(this->_M_pointer);
    }

    void _M_acquire(_Tp *ptr) {
        this->_M_pointer = ptr;
        ++this->_M_counter();
    }

    void _M_release() {
        if (--this->_M_counter() == 0) {
            this->_M_deleter();
        }
    }

    explicit shared_ptr(_Tp *ptr) noexcept : _M_pointer(ptr) {
        this->_M_acquire(ptr);
    }

public:
    shared_ptr(const shared_ptr &other)
        noexcept : shared_ptr(other._M_pointer) {}

    ~shared_ptr() { this->_M_release(); }

    auto operator=(const shared_ptr &other) noexcept -> shared_ptr & {
        if (this != &other) {
            this->_M_release();
            this->_M_acquire(other._M_pointer);
        }
        return *this;
    }

    static auto unsafe_make(_Tp *ptr) -> shared_ptr {
        return shared_ptr(ptr);
    }

    auto operator*() const noexcept -> _Tp & { return *this->_M_pointer; }
    auto operator->() const noexcept -> _Tp * { return this->_M_pointer; }

private:
    _Tp *_M_pointer;
}; // struct shared_ptr

} // namespace dark
