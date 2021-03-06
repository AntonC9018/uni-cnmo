#pragma once
#include "util.h"

namespace Root_Finding
{
    template<typename Function>
    inline bool bisection_conditions_met(Function f, double start, double end)
    {
        return start < end && signbit(f(start)) != signbit(f(end)); 
    }

    template<typename Function>
    double bisection(Function f, double start, double end, 
        double tolerance, size_t max_iters, bool* error)
    {
        RF_ERROR_IF_FALSE(bisection_conditions_met(f, start, end));
        size_t i = 0;
        bool sign_start = signbit(f(start));

        while (i < max_iters)
        {
            double middle = (start + end) / 2;
            if (f(middle) < tolerance && (end - start) < tolerance * 2)
            {
                return middle;
            }
            if (signbit(f(middle)) == sign_start) 
            {
                start = middle;
            }
            else
            {
                end = middle;
            }
            i++;
        }

        RF_ERROR();
    }
}