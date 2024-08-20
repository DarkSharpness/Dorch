#pragma once
#include <cstdint>

namespace dark {

enum class Device : std::uint8_t { CPU, CUDA, };

inline constexpr auto kCPU = Device::CPU;
inline constexpr auto kCUDA = Device::CUDA;

} // namespace dark
