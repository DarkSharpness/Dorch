#pragma once
#include <cstdint>

namespace dark {

enum class DataType : std::uint8_t {
    INT8,
    INT16,
    INT32,
    INT64,
    FLOAT32,
    FLOAT64,
};

inline constexpr auto kInt8    = DataType::INT8;
inline constexpr auto kInt16   = DataType::INT16;
inline constexpr auto kInt32   = DataType::INT32;
inline constexpr auto kInt64   = DataType::INT64;
inline constexpr auto kFloat32 = DataType::FLOAT32;
inline constexpr auto kFloat64 = DataType::FLOAT64;

} // namespace dark
