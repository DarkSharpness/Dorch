#include <iostream>
#include <vector>

signed main() {
    std::vector <int> v;
    auto val = std::move(v);
    v.clear();
    return 0;
}