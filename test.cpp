#include <vector>
decltype(auto) func() {
    return std::vector<int> {1}[0];
}

signed main() {
    std::vector <int> v;
    auto val = std::move(v);
    v.clear();
    return 0;
}