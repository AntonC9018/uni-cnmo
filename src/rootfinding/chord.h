#pragma once
#include "util.h"

namespace Root_Finding
{
    template<typename Function>
    inline bool chord_conditions_met(Function f, double start, double end)
    {
        return signbit(f(start)) != signbit(f(end)); 
    }

    template<typename Function>
    double chord(Function f, Function f_second_derivative, double start, double end, 
        double tolerance, size_t max_iters, bool* error)
    {
        RF_ERROR_IF_FALSE(chord_conditions_met(f, start, end));

        // Whether the start is in the trough or outside one.
        bool side_start = signbit(f(start)) == signbit(f_second_derivative(start));

        // Always start in the trough.
        double x     = side_start ? end : start;

        // Rel stands for relative. The approximations 
        // are always calculated relative to this value.
        double x_rel = side_start ? start : end;
        double f_rel = f(x_rel);

        size_t i = 0;

        while (i < max_iters)
        {
            double f_x = f(x);

            /*  x_new
               x  \  /|
                \  |/ | < f(rel)
            _____._.__|____________ x = 0 
                  /_|_| < f(x)
                 |_|  ^
                  ^  x_rel  
                 dx
               
               dx / f(x) = (x_rel - x) / (f(rel) - f(x))
               x_new = x + dx
            */
            double dx = (x - x_rel) * f_x / (f_rel - f_x);

            if (abs(f_x) < tolerance && abs(dx) < tolerance)
                return x;
            
            x += dx;
            i++;
        }

        RF_ERROR();
    }
}