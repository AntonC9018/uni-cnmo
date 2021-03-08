#pragma once
#include "shared.h"

namespace Root_Finding
{
    template<typename Function>
    inline bool secant_conditions_met(Function& f, const Interval i)
    {
        return f(i.start) != f(i.end);
    }

    template<typename Function>
    double secant(
        Function& f, 
        const double x0, 
        const double x1, 
        Error_Data* error_data, 
        Profiler* profiler = &_std_profiler)
    {
        size_t i = 0;
        double x        = x1;
        double f_x_prev = f(x0);
        double dx       = x1 - x0;

        while (i < error_data->max_iters)
        {
            double f_x = f(x);

            profiler->num_iters++;

            if (abs(f_x) < error_data->tolerance && abs(dx) < error_data->tolerance)
                return x;

            dx *= f_x / (f_x_prev - f_x);
            x += dx;
            f_x_prev = f_x; 
            i++;
        }

        RF_ITERATIONS_ERROR();
    }

    template<typename Function, typename Derivative>
    double secant_enhanced_start(
        Function& f, 
        Derivative& f_second_derivative, 
        const Interval inter,
        Error_Data* error_data, 
        Profiler* profiler = &_std_profiler)
    {
        RF_INITIAL_CHECK(secant_conditions_met(f, inter));

        double x0;
        double x1;

        // Choose the place where the derivative is bigger. Imagine x^2 graph.
        // This condition will always get you on the steep slope rather then in the trough.
        if (signbit(f(inter.start)) == signbit(f_second_derivative(inter.start)))
        {
            x0 = inter.start;
            x1 = inter.start + error_data->tolerance;
        }
        else
        {
            x0 = inter.end;
            x1 = inter.end - error_data->tolerance;
        }

        return secant(f, x0, x1, error_data, profiler);
    }
}