#pragma once
#include <cparse/shunting-yard.h>
#include <strlib.h>

#include "func.h"

#define countof(arr) sizeof((arr)) / sizeof((arr)[0])

namespace Builtin
{
    const Func funcs[]
    {
        make_func(str_lit("2 * sin(3 * x) - ln(x^3 - 1) + 4"), 2, 9),
        make_func(str_lit("sin(pi * x / 6) - cos(x - 1)"), -7, 8),
        make_func(str_lit("e^-x - x^3 + 8 * cos(4 * x)"), -4, 4),
        make_func(str_lit("x^6 - 5.5 * x^5 + 6.18 * x^4 + 16.54 * x^3 - 56.9592 * x^2 + 55.9872 * x - 19.3156"), -3, 4),
        make_func(str_lit("x^6 - 0.7 * x^5 - 8.7 * x^4 + 5.58 * x^3 + 22.356 * x^2 - 8.39808 * x"), -3, 4),
        make_func(str_lit("x^6 - 2.4 * x^5 - 18.27 * x^4 + 23.216 * x^3 + 115.7 * x^2 - 19.5804 * x - 164.818"), -3, 4)
    };

    const int func_count = countof(funcs);

    // Returns a token map with some basic constants
    inline TokenMap make_token_map()
    {
        TokenMap token_map;
        token_map["pi"] = M_PI;
        token_map["e"]  = M_E;
        return token_map;
    }
}