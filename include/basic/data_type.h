#pragma once
#include <cstdint>

namespace dark {

enum class DataType : std::uint8_t {
    INT8, INT16, INT32, INT64, FLOAT32, FLOAT64,
};

inline constexpr auto kINT8 = DataType::INT8;
inline constexpr auto kINT16 = DataType::INT16;
inline constexpr auto kINT32 = DataType::INT32;
inline constexpr auto kINT64 = DataType::INT64;
inline constexpr auto kFLOAT32 = DataType::FLOAT32;
inline constexpr auto kFLOAT64 = DataType::FLOAT64;

} // namespace dark
