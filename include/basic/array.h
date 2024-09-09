#pragma once
#include <span>
#include <vector>

namespace dark {

using IntArray     = std::vector<int>;
using IntArrayView = struct _Span : std::span<int> {
    using std::span<int>::span;
    operator IntArray() const { return IntArray(begin(), end()); }
};

} // namespace dark
