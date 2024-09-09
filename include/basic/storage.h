#pragma once
#include "utility/shared.h"
#include <cstddef>
#include <functional>
#include <memory>

namespace dark {

struct Storage;

namespace detail {

struct StorageView {
private:
    friend struct ::dark::Storage;
    friend struct shared_counter;

    using _Deleter_t = std::function<void(void *)>;
    using _Manage_t  = std::unique_ptr<void, _Deleter_t>;

    explicit StorageView(void *data, _Deleter_t deleter) :
        _M_ref_count(0),
        _M_data(data, std::move(deleter)) {}

    struct DummyTag {};

    explicit StorageView() noexcept : _M_ref_count(1), _M_data(nullptr, [](void *) {}) {}

    inline static auto _S_dummy() -> StorageView & {
        // To suppress the unused variable warning.
        static auto instance = StorageView{};
        static_cast<void>(instance._M_ref_count);
        return instance;
    }

    StorageView(const StorageView &)    = delete;
    auto operator=(const StorageView &) = delete;

    std::size_t _M_ref_count;
    const _Manage_t _M_data;
};

using StorageBase = shared_ptr<StorageView>;

} // namespace detail

/**
 * The manager type fore the inner storage.
 */
struct Storage : private detail::StorageBase {
private:
    using Base_t    = detail::StorageBase;
    using View_t    = detail::StorageView;
    using Deleter_t = View_t::_Deleter_t;

public:
    explicit Storage() : Base_t(Base_t::from_ptr(&View_t::_S_dummy())), _M_data(), _M_size() {}

    explicit Storage(void *data, std::size_t size, Deleter_t deleter) :
        Base_t(Base_t::from_ptr(new View_t{data, std::move(deleter)})),
        _M_data(data),
        _M_size(size) {}

    Storage(const Storage &other)                     = default;
    auto operator=(const Storage &other) -> Storage & = default;

    auto data() const -> void * { return this->_M_data; }
    auto size() const -> std::size_t { return this->_M_size; }

private:
    void *_M_data;
    std::size_t _M_size;
};

} // namespace dark
