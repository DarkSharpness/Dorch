#pragma once
#include "basic/array.h"
#include "data_type.h"
#include "device.h"
#include "storage.h"
#include <cstddef>
#include <memory>

namespace dark {

struct TensorBase;

struct Tensor;

struct TensorBase {
    Storage_View    storage;
    DataType        dtype;
    Device          device;

    // Shape of the tensor
    IntArray        size;
    IntArray        stride;

    std::function<void(Tensor)> grad_fn;

    bool requires_grad  : 1;
    bool is_leaf        : 1;

private:
    friend struct Tensor;
    std::size_t ref_count;
};

struct Tensor : private std::shared_ptr<TensorBase> {
};

} // namespace dark
