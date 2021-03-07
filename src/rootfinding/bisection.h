#pragma once
#include "util.h"

namespace Root_Finding
{
    template<typename Function>
    inline bool bisection_conditions_met(const Function f, const Interval i)
    {
        return i.start < i.end && signbit(f(i.start)) != signbit(f(i.end)); 
    }

    template<typename Function>
    double bisection(
        const Function f, 
        Interval inter, 
        Error_Data* error_data, 
        Profiler* profiler = &_std_profiler)
    {
        RF_INITIAL_CHECK(bisection_conditions_met(f, inter));
        size_t i = 0;
        bool sign_start = signbit(f(inter.start));

        while (i < error_data->max_iters)
        {
            profiler->num_iters++;

            double middle = (inter.start + inter.end) / 2;
            double length = inter.end - inter.start;

            if (f(middle) < error_data->tolerance 
                && length < error_data->tolerance * 2)
            {
                return middle;
            }
            if (signbit(f(middle)) == sign_start) 
            {
                inter.start = middle;
            }
            else
            {
                inter.end = middle;
            }
            i++;
        }

        RF_ITERATIONS_ERROR();
    }
}