#include <cparse/shunting-yard.h>

#define countof(arr) sizeof((arr)) / sizeof((arr)[0])

namespace Builtin
{
    const char* expression_strings[]
    {
        "2 * sin(3 * x) - ln(x^3 - 1) + 4",
        "sin(pi * x / 6) - cos(x - 1)",
        "e^-x - x^3 + 8 * cos(4 * x)",
        "x^6 - 5.5 * x^5 + 6.18 * x^4 + 16.54 * x^3 - 56.9592 * x^2 + 55.9872 * x - 19.3156",
        "x^6 - 0.7 * x^5 - 8.7 * x^4 + 5.58 * x^3 + 22.356 * x^2 - 8.39808 * x",
        "x^6 - 2.4 * x^5 - 18.27 * x^4 + 23.216 * x^3 + 115.7 * x^2 - 19.5804 * x - 164.818"
    };

    calculator calculators[countof(expression_strings)]; 

    void compile()
    {
        for (int i = 0; i < countof(expression_strings); i++)
        {
            calculators[i].compile(expression_strings[i]);
        }
    }

    // Returns a token map with some basic constants
    TokenMap make_token_map()
    {
        TokenMap token_map;
        token_map["pi"] = M_PI;
        token_map["e"]  = M_E;
        return token_map;
    }
}