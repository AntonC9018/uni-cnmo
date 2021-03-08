#pragma once
#include <strlib.h>

namespace Root_Finding
{
    enum Option
    {
        BISECTION = 0, CHORD = 1, NEWTON = 2, SECANT = 3
    };

    const str_view_t option_text[]
    {
        str_lit("Bisection"),
        str_lit("Chord"),
        str_lit("Newton"),
        str_lit("Secant"),
    }; 
}