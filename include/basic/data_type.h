#pragma once
#include <cstdint>

namespace dark {

enum class DataType : std::uint8_t {
    INT8, INT16, INT32, INT64, FLOAT32, FLOAT64,
};

} // namespace dark
