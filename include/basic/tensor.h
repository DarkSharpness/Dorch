#pragma once
#include "basic/array.h"
#include "basic/data_type.h"
#include "basic/device.h"
#include "basic/storage.h"
#include "utility/shared.h"
#include <cstddef>
#include <functional>

namespace dark {

struct Tensor;

namespace detail {

struct TensorView {
private:
    friend struct shared_counter;
    friend struct ::dark::Tensor;

public:
    explicit TensorView(
        const Storage &storage, IntArray size, IntArray stride, Device device, DataType data_type
    ) :
        _M_ref_count(0),
        _M_size(std::move(size)),
        _M_stride(std::move(stride)),
        _M_device(device),
        _M_dtype(data_type),
        _M_storage(storage) {
        static_cast<void>(this->_M_ref_count); // To suppress the warning
    }

    auto get_storage() -> Storage & { return this->_M_storage; }

    auto get_stride() -> IntArrayView { return this->_M_stride; }

    auto get_size() -> IntArrayView { return this->_M_size; }

    auto get_device() -> Device { return this->_M_device; }

    auto get_dtype() -> DataType { return this->_M_dtype; }

private:
    std::size_t _M_ref_count; // Reference count
    IntArray _M_size;         // Size of the tensor
    IntArray _M_stride;       // Stride of the tensor
    Device _M_device;         // Device of the tensor
    DataType _M_dtype;        // Data type of the tensor
    Storage _M_storage;       // Internal storage
};

using TensorBase = shared_ptr<TensorView>;

} // namespace detail

struct Tensor : private detail::TensorBase {
private:
    using Base_t  = detail::TensorBase;
    using View_t  = detail::TensorView;
    using Deleter = std::function<void(std::byte *)>;

public:
    explicit Tensor(
        const Storage &storage, IntArray array, IntArray stride, Device device, DataType data_type
    ) :
        Base_t(Base_t::from_ptr(
            new View_t{storage, std::move(array), std::move(stride), device, data_type}
        )) {}

    using Base_t::operator->;
    using Base_t::operator*;
};

} // namespace dark
