#pragma once
#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>

namespace dark {

struct Storage {
private:
    using _Deleter_t = std::function<void(std::byte*)>;
    using _Manage_t = std::unique_ptr<std::byte[], _Deleter_t>;
    friend struct Storage_View;

    explicit Storage(std::size_t size):
        _M_data(new std::byte[size], std::default_delete<std::byte[]>()),
        _M_size(size), _M_ref_count(0) {}
 
    explicit Storage(std::byte *data, std::size_t size, std::function<void(std::byte*)> deleter):
        _M_data(data, std::move(deleter)), _M_size(size), _M_ref_count(0) {}

    struct DummyTag {};

    explicit Storage(DummyTag) : _M_data(nullptr, [](void *) {
        throw std::runtime_error("Dummy storage cannot be deleted"); 
    }), _M_size(0), _M_ref_count(1) {}

    Storage(const Storage &) = delete;
    auto operator=(const Storage &) = delete;

    _Manage_t   _M_data;
    std::size_t _M_size;
    std::size_t _M_ref_count;
};

struct Storage_View {
    inline static Storage dummy { Storage::DummyTag{} };

private:
    explicit Storage_View(Storage *storage) {
        _M_acquire(storage);
    }

    void _M_acquire(Storage *storage) {
        _M_storage = storage;
        _M_storage->_M_ref_count++;
    }

    void _M_release() {
        if (_M_storage->_M_ref_count > 0) {
            _M_storage->_M_ref_count--;
        } else {
            delete _M_storage;
        }
    }

    void _M_steal(Storage_View &other) {
        _M_storage = other._M_storage;
        other._M_storage = &dummy;
    }

public:
    explicit Storage_View(std::byte *data, std::size_t size, std::function<void(std::byte*)> deleter) :
        Storage_View(new Storage(data, size, std::move(deleter))) {}

    explicit Storage_View(std::size_t size) : Storage_View(new Storage(size)) {}

    Storage_View(const Storage_View &other) : Storage_View(other._M_storage) {}

    Storage_View(Storage_View &&other) noexcept { _M_steal(other); }

    auto operator=(const Storage_View &other) -> Storage_View & {
        if (this == &other) return *this;
        _M_release();
        _M_acquire(other._M_storage);
        return *this;
    }

    auto operator=(Storage_View &&other) noexcept -> Storage_View & {
        if (this == &other) return *this;
        _M_release();
        _M_steal(other);
        return *this;
    }

    ~Storage_View() {
        _M_release();
    }

    auto data() const -> std::byte * {
        return _M_storage->_M_data.get();
    }
    auto nbytes() const -> std::size_t {
        return _M_storage->_M_size;
    }

private:
    Storage * _M_storage;
};

} // namespace dark
