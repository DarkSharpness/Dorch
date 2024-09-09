#pragma once
#include "backward/node.h"
#include "basic/tensor.h"
#include "function/empty.h"
#include "utility/assume.h"
#include <type_traits>

namespace dark::CPU {

template <typename _Scalar>
    requires std::is_arithmetic_v<_Scalar>
struct TensorAdd {
    static auto forward(TensorList input) -> Tensor {
        assume(input.size() == 2, "Input size must be 2");
        auto &s0 = input[0]->get_storage();
        auto &s1 = input[1]->get_storage();
        assume(s0.size() == s1.size(), "Input size must be same");
        const auto size = s0.size() / sizeof(_Scalar);
        auto *p1        = static_cast<_Scalar *>(s0.data());
        auto *p2        = static_cast<_Scalar *>(s1.data());
        auto tensor     = empty_like(input[0]);
        auto *p3        = static_cast<_Scalar *>(tensor->get_storage().data());
        for (size_t i = 0; i < size; ++i)
            p3[i] = p1[i] + p2[i];
        return tensor;
    }
};

} // namespace dark::CPU
