#include "basic/device.h"
#include "basic/storage.h"
#include "basic/tensor.h"
#include <cstddef>
#include <iostream>

[[maybe_unused]] static void test_view() {
    using dark::StorageView;
    constexpr auto kSize = 10;
    std::byte *pointer = new std::byte[kSize]; 
    StorageView storage { pointer, kSize, [](std::byte *p) { delete []p; } };
    auto *data = storage.data();
    auto  size = storage.size();
    std::cout << "size: " << size << '\n';
    for (std::size_t i = 0; i < size; ++i)
        data[i] = std::byte(i);
    for (std::size_t i = 0; i < size; ++i)
        std::cout << std::to_integer<int>(data[i]) << ' ';
    std::cout << std::endl;
}

[[maybe_unused]] static void test_tensor() {
    using dark::Tensor;
    using dark::StorageView;

    Tensor tensor {
        StorageView { new std::byte[10], 10, [](std::byte *p) { delete []p; } },
        { 2, 5 },
        { 1, 1 },
        dark::kCPU,
        dark::kInt32
    };

    auto tmp = tensor;
    tmp = tensor;
    tensor = tmp;
    auto fuck = std::move(tmp);
}

int main() {
    test_view();
    test_tensor();
    std::cout << "Passed\n";
    return 0;
}
