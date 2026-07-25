#include <func/func.h>
#include <mydefines.h>

namespace Poly
{
    static Expression_Func funcs[5] {};

    const size_t func_count = countof(funcs);

    static inline void init_funcs()
    {
        static bool initialized = false;
        if (initialized)
            return;

        func_init(&funcs[0], str_lit("x * exp(2 * sin(x))"), -3, 3);
        func_init(&funcs[1], str_lit("sin(x) * cos(x)"), -6, 6);
        func_init(&funcs[2], str_lit("sin(pi * x / 6) * cos(x - 1)"), -4, 4);
        func_init(&funcs[3], str_lit("exp(-x) - x^3 + 8 * cos(4 * x)"), -4, 4);
        func_init(&funcs[4], str_lit("x^3 - 5 * atan(x)"), -2, 2);

        initialized = true;
    }
}
