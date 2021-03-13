#include <func/func.h>

namespace Root_Finding
{
    static Expression_Func funcs[]
    {
        func_make(str_lit("2 * sin(3 * x) - ln(x^3 - 1) + 4"), 2, 9),
        func_make(str_lit("sin(pi * x / 6) - cos(x - 1)"), -7, 8),
        func_make(str_lit("exp(-x) - x^3 + 8 * cos(4 * x)"), -4, 4),
        func_make(str_lit("x^6 - 5.5 * x^5 + 6.18 * x^4 + 16.54 * x^3 - 56.9592 * x^2 + 55.9872 * x - 19.3156"), -3, 4),
        func_make(str_lit("x^6 - 0.7 * x^5 - 8.7 * x^4 + 5.58 * x^3 + 22.356 * x^2 - 8.39808 * x"), -3, 4),
        func_make(str_lit("x^6 - 2.4 * x^5 - 18.27 * x^4 + 23.216 * x^3 + 115.7 * x^2 - 19.5804 * x - 164.818"), -3, 4)
    };

    const size_t func_count = countof(funcs);
}