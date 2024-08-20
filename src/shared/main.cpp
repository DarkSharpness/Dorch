#include "basic/shared.h"
#include <iostream>

struct Package {
    int data;
    int counter;
};

using dark::shared_ptr;

struct Deleter {
    constexpr void operator()(Package* ptr) const {
        std::cout << "Deleter called\n";
        delete ptr;
    }
};

signed main() {
    auto ptr = shared_ptr<Package, Deleter>::unsafe_make(new Package{ .data= 1, .counter= 0 });
    auto ptr2 = ptr;
    auto ptr3 = ptr;
    ptr3 = ptr2;
    return 0;
}
