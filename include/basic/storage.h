#pragma once
#include "basic/shared.h"
#include <cstddef>
#include <functional>
#include <memory>

namespace dark {

struct Storage_View;

namespace detail {
    struct counter_storage;
    struct default_storage;
} // namespace detail

struct Storage {
private:
    friend struct detail::counter_storage;
    friend struct detail::default_storage;
    friend struct Storage_View;

    using _Deleter_t = std::function<void(std::byte*)>;
    using _Manage_t = std::unique_ptr<std::byte[], _Deleter_t>;

    explicit Storage(std::byte *data, std::size_t size, _Deleter_t deleter) :
        _M_data(data, std::move(deleter)), _M_size(size), _M_ref_count(0) {}

    struct DummyTag {};

    explicit Storage(DummyTag) : _M_data(nullptr, [](void *){}), _M_size(0), _M_ref_count(0) {}

    Storage(const Storage &) = delete;
    auto operator=(const Storage &) = delete;

    _Manage_t   _M_data;
    std::size_t _M_size;
    std::size_t _M_ref_count;
};

namespace detail {

    struct counter_storage {
        constexpr auto operator()(Storage &storage) -> std::size_t & {
            return storage._M_ref_count;
        }
    };

    struct default_storage {
        constexpr operator Storage() {
            return Storage { Storage::DummyTag{} };
        }
    };

    using Storage_Base = shared_ptr <
        Storage,
        std::default_delete<Storage>,
        counter_storage,
        default_storage
    >;

} // namespace detail

struct Storage_View : private detail::Storage_Base {
private:
    using Base_t = detail::Storage_Base;
    using Deleter_t = std::function<void(std::byte*)>;

public:
    explicit Storage_View(std::byte *data, std::size_t size, Deleter_t deleter)
        : Base_t(Base_t::unsafe_make(new Storage(data, size, std::move(deleter)))) {}

    Storage_View(const Storage_View &) = default;
    auto operator=(const Storage_View &) -> Storage_View & = default;

    auto data() const -> std::byte * {
        return (*this)->_M_data.get();
    }
    auto size() const -> std::size_t {
        return (*this)->_M_size;
    }
};

} // namespace dark
