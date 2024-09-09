#pragma once
#include <memory>

namespace dark {

namespace detail {

struct shared_counter {
    /**
     * Get the counter of the shared object.
     */
    template <typename _Tp>
    static constexpr auto &operator()(_Tp &val) noexcept {
        return val._M_ref_count;
    }
};

} // namespace detail

// clang-format off

/**
 * A cpu-side shared pointer implementation, which allows intrusive reference
 * counting. Such a shared pointer always holds a non-null pointer to the
 * object.
 */
template <
    typename _Tp,
    typename _Deleter = std::default_delete<_Tp>,
    typename _Counter = detail::shared_counter
>
// clang-format on
struct shared_ptr {
private:
    auto _M_clean() { return _M_deleter(this->_M_pointer); }

    auto &_M_count() { return _M_counter(*this->_M_pointer); }
    auto &_M_count() const { return _M_counter(*this->_M_pointer); }

    auto _M_acquire(_Tp *ptr) -> void {
        this->_M_pointer = ptr;
        ++this->_M_count();
    }

    auto _M_release() -> void {
        if (--this->_M_count() == 0)
            this->_M_clean();
    }

    explicit shared_ptr(_Tp *ptr) noexcept { this->_M_acquire(ptr); }

public:
    shared_ptr(const shared_ptr &other) noexcept : shared_ptr(other._M_pointer) {}

    ~shared_ptr() { this->_M_release(); }

    /* Copy-and-swap idiom.  */
    auto operator=(const shared_ptr &other) noexcept -> shared_ptr & {
        shared_ptr(other).swap(*this);
        return *this;
    }

    /* Named constructor, for safety.  */
    static auto from_ptr(_Tp *ptr) -> shared_ptr { return shared_ptr(ptr); }

    /* Return the inner raw pointer.  */
    auto get() const noexcept -> _Tp * { return this->_M_pointer; }

    auto operator*() const noexcept -> _Tp & { return *this->_M_pointer; }
    auto operator->() const noexcept -> _Tp * { return this->_M_pointer; }

    /* Swap the content of two shared pointers.  */
    auto swap(shared_ptr &other) noexcept -> void { std::swap(this->_M_pointer, other._M_pointer); }

    auto use_count() const noexcept -> std::size_t { return this->_M_count(); }

private:
    _Tp *_M_pointer;
    [[no_unique_address]] _Deleter _M_deleter;
    [[no_unique_address]] _Counter _M_counter;
};

} // namespace dark
