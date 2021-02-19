#pragma once
#include <cparse/shunting-yard.h>
#include <strlib.h>


struct Func
{
    str_view_t text;
    calculator calculator;
    double upper_bound;
    double lower_bound;
};

inline Func make_func(str_view_t text, double lower, double upper)
{
    Func func;
    func.text        = text;
    func.calculator  = text.chars;
    func.lower_bound = lower;
    func.upper_bound = upper;
    return func;
}