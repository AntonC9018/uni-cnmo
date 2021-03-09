#pragma once
#include "shared.h"

namespace Root_Finding
{
    template<typename Derivative>
    inline bool newton_conditions_met(
        Derivative f_derivative, const double x_initial_guess)
    {
        return f_derivative(x_initial_guess) != 0;
    }

    template<typename Function, typename Derivative>
    double newton(
        Function& f, 
        Derivative f_derivative, 
        const double x_initial_guess, 
        Error_Data* error_data,
        Profiler* profiler = &_std_profiler)
    {
        RF_INITIAL_CHECK(newton_conditions_met(f_derivative, x_initial_guess));

        size_t i = 0;
        double x = x_initial_guess;

        while (i < error_data->max_iters)
        {
            double dx = f(x) / f_derivative(x);
            x = x - dx;
            
            profiler->num_iters++;

            if (abs(dx) < error_data->tolerance)
                return x;

            i++;
        }

        RF_ITERATIONS_ERROR();
    }

    template<typename Function, typename Derivative, typename Second_Derivative>
    double newton_enhanced_start(
        Function& f, 
        Derivative f_derivative, 
        Second_Derivative f_second_derivative,
        const Interval inter,
        Error_Data* error_data,
        Profiler* profiler = &_std_profiler)
    {
        // Start on the side rather than in a trough
        const double x_initial = signbit(f(inter.start)) == signbit(f_second_derivative(inter.start))
            ? inter.start : inter.end; 
        
        return newton(f, f_derivative, x_initial, error_data, profiler); 
    }
}