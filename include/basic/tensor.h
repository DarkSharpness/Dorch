#pragma once
#include "basic/array.h"
#include "basic/data_type.h"
#include "basic/device.h"
#include "basic/shared.h"
#include "basic/storage.h"
#include <cstddef>
#include <functional>
#include <memory>

namespace dark {

namespace detail {
    struct counter_tensor;
    struct default_tensor;
} // namespace detail

struct TensorView;

struct Tensor;

struct TensorView {
private:
    friend struct detail::counter_tensor;
    friend struct detail::default_tensor;
    friend struct Tensor;

public:
    explicit TensorView(
        const StorageView &storage,
        IntArray size,
        IntArray stride,
        Device device,
        DataType data_type)
        : _M_storage(storage),
          _M_size(std::move(size)),
          _M_stride(std::move(stride)),
          _M_device(device),
          _M_data_type(data_type) {}

    auto get_stride() -> IntArrayView {
        return this->_M_stride;
    }

    auto get_size() -> IntArrayView {
        return this->_M_size;
    }

    auto get_device() -> Device {
        return this->_M_device;
    }

    auto get_data_type() -> DataType {
        return this->_M_data_type;
    }

private:
    StorageView _M_storage;

    IntArray    _M_size;
    IntArray    _M_stride;

    Device      _M_device;
    DataType    _M_data_type;

    std::size_t _M_ref_count;
};

namespace detail {

    struct counter_tensor {
        constexpr auto operator()(TensorView &tensor) -> std::size_t & {
            return tensor._M_ref_count;
        }
    };

    struct default_tensor {
        constexpr operator TensorView() {
            return TensorView {
                StorageView { StorageView::DummyTag{} },
                {},
                {},
                {},
                {},
            };
        }
    };

    using TensorBase = shared_ptr <
        TensorView,
        std::default_delete<TensorView>,
        counter_tensor,
        default_tensor
    >;

} // namespace detail

struct Tensor : private detail::TensorBase {
private:
    using Base_t = detail::TensorBase;
    using Deleter = std::function<void(std::byte *)>;

public:
    explicit Tensor(
        const StorageView &storage,
        IntArray array,
        IntArray stride,
        Device device,
        DataType data_type)
        : Base_t(unsafe_make(
            new TensorView(
                storage,
                std::move(array),
                std::move(stride),
                device,
                data_type
            )
        )) {}
};

} // namespace dark
