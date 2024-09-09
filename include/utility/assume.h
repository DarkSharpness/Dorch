#include <format>
#include <iostream>
#include <source_location>

namespace dark {

template <typename _Tp, typename... _Args>
struct assume {
    explicit assume(
        _Tp &&condition, std::format_string<_Args...> fmt = "", _Args &&...args,
        std::source_location location = std::source_location::current()
    ) {
        if (!condition) {
            std::cerr << "assume failed: " << location.file_name() << ":" << location.line() << ": "
                      << location.function_name() << ": ";
            std::cerr << std::format(fmt, std::forward<_Args>(args)...) << std::endl;
        }
    }
};

template <typename _Tp, typename _Fmt, typename... _Args>
assume(_Tp &&, _Fmt &&, _Args &&...) -> assume<_Tp, _Args...>;

} // namespace dark
