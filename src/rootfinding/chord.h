#pragma once
#include "util.h"

namespace Root_Finding
{
    template<typename Function>
    inline bool chord_conditions_met(const Function f, const Interval i)
    {
        return signbit(f(i.start)) != signbit(f(i.end)); 
    }

    template<typename Function>
    double chord(
        const Function f, 
        const Function f_second_derivative, 
        const Interval inter, 
        Error_Data* error_data, 
        Profiler* profiler = &_std_profiler)
    {
        RF_INITIAL_CHECK(chord_conditions_met(f, inter));

        // Whether the start is in the trough or outside one.
        bool side_start = signbit(f(inter.start)) == signbit(f_second_derivative(inter.start));

        // Always start in the trough.
        double x = side_start ? inter.end : inter.start;

        // Rel stands for relative. The approximations 
        // are always calculated relative to this value.
        double x_rel = side_start ? inter.start : inter.end;
        double f_rel = f(x_rel);

        size_t i = 0;

        while (i < error_data->max_iters)
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

            profiler->num_iters++;

            if (abs(f_x) < error_data->tolerance 
                && abs(dx) < error_data->tolerance)
                return x;
            
            x += dx;
            i++;
        }

        RF_ITERATIONS_ERROR();
    }
}