#pragma once
#include "util.h"

namespace Root_Finding
{
    template<typename Function>
    inline bool newton_conditions_met(Function f_derivative, double x_initial_guess)
    {
        return f_derivative(x_initial_guess) != 0;
    }

    template<typename Function>
    double newton(Function f, Function f_derivative, double x_initial_guess, 
        double tolerance, size_t max_iters, bool* error)
    {
        RF_ERROR_IF_FALSE(newton_conditions_met(f_derivative, x_initial_guess));

        size_t i = 0;
        double x = x_initial_guess;

        while (i < max_iters)
        {
            double dx = f(x) / f_derivative(x);
            x = x - dx;
            
            if (abs(dx) < tolerance)
                return x;

            i++;
        }

        RF_ERROR();
    }
}