#include "utility/shared.h"
#include <iostream>

struct Package {
    int data;
    int _M_ref_count;
};

using dark::shared_ptr;

struct Deleter {
    constexpr void operator()(Package *ptr) const {
        std::cout << "Deleter called\n";
        delete ptr;
    }
};

signed main() {
    auto ptr  = shared_ptr<Package, Deleter>::from_ptr(new Package{.data = 1, ._M_ref_count = 0});
    {
        auto ptr2 = ptr;
        auto ptr3 = ptr;
        ptr3      = ptr2;
    }
    std::cout << "ptr count: " << ptr.use_count() << '\n';
    for (int i = 0 ; i < 100 ; ++i) {
        auto tmp = ptr;
        ptr.swap(tmp);
    }
    std::cout << "ptr count: " << ptr.use_count() << '\n';
    return 0;
}
