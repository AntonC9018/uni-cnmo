#pragma once
#include "util.h"

namespace Root_Finding
{
    template<typename Function>
    inline bool secant_conditions_met(Function f, double x0, double x1)
    {
        return f(x0) != f(x1);
    }

    template<typename Function>
    double secant(Function f, double x0, double x1, 
        const double tolerance, const size_t max_iters, bool* error)
    {
        size_t i = 0;
        double x_prev   = x0;
        double x        = x1;
        double f_x_prev = f(x0);
        double dx       = x1 - x0;

        while (i < max_iters)
        {
            double f_x = f(x);

            if (abs(f_x) < tolerance && abs(dx) < tolerance)
                return x;

            dx *= f_x / (f_x_prev - f_x);
            x_prev = x; x += dx;
            f_x_prev = f_x; 
            i++;
        }

        RF_ERROR();
    }

    template<typename Function>
    double secant_enhanced_start(Function f, Function f_second_derivative, 
        double start, double end, const double tolerance, const size_t max_iters, bool* error)
    {
        RF_ERROR_IF_FALSE(secant_conditions_met(f, start, end));

        double x0;
        double x1;

        // Choose the place where the derivative is bigger. Imagine x^2 graph.
        // This condition will always get you on the steep slope rather then in the trough.
        if (signbit(f(start)) == signbit(f_second_derivative(start)))
        {
            x0 = start;
            x1 = start + tolerance;
        }
        else
        {
            x0 = end;
            x1 = end - tolerance;
        }

        return secant(f, x0, x1, tolerance, max_iters, error);
    }
}