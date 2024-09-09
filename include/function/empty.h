#pragma once
#include "basic/storage.h"
#include "basic/tensor.h"

namespace dark::CPU {

inline auto empty_like(Tensor tensor) -> Tensor {
    const auto &storage = tensor->get_storage();
    const auto size = storage.size();
    const auto data = new char[size];
    return Tensor {
        Storage { data, size, [](void *ptr) { delete[] static_cast<char *>(ptr); }},
        tensor->get_size(),
        tensor->get_stride(),
        tensor->get_device(),
        tensor->get_dtype() 
    };
}


}