#include "basic/storage.h"
#include <cstddef>
#include <iostream>

int main() {
    using dark::Storage_View;
    constexpr auto kSize = 10;
    std::byte *pointer = new std::byte[kSize]; 
    Storage_View storage { pointer, kSize, [](std::byte *p) { delete []p; } };
    auto *data = storage.data();
    auto  size = storage.size();
    std::cout << "size: " << size << '\n';
    for (std::size_t i = 0; i < size; ++i)
        data[i] = std::byte(i);
    for (std::size_t i = 0; i < size; ++i)
        std::cout << std::to_integer<int>(data[i]) << ' ';
    std::cout << '\n';
    return 0;
}
