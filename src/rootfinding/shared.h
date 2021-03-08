#pragma once
#include <vector>
#include <stdlib.h>
#include <strlib.h>
#include <math.h>
#include "../profiler.h"

namespace Root_Finding
{
    struct Error_Data
    {
        double tolerance;
        size_t max_iters;
        str_view_t error_message;
    };

    struct Interval
    {
        double start;
        double end;
    };

    const str_view_t INITIAL_CHECK_FAILED_MESSAGE = str_lit("Initial check has failed");
    const str_view_t TOO_MANY_ITERATIONS_MESSAGE  = str_lit("The max number of iterations has been exceeded");

#define RF_INITIAL_CHECK(expr)\
    if (!(expr)) { error_data->error_message = INITIAL_CHECK_FAILED_MESSAGE; return 0; } \
    else { error_data->error_message = STR_NULL; }

#define RF_ITERATIONS_ERROR() { error_data->error_message = TOO_MANY_ITERATIONS_MESSAGE; return 0; }

    inline double abs(double a) { return signbit(a) ? -a : a; }

    // Numerically differentiate function f at point x with step delta
    template<typename Function>
    inline double differentiate(Function& f, double x, double delta)
    {
        return (f(x + delta) - f(x)) / delta;
    }

    template<typename Function>
    std::vector<double> localize(Function& f, double start, double end, double step)
    {
        std::vector<double> result;

        double x = start;
        double sign_f_x = signbit(f(x));

        while (x + step <= end)
        {
            double x_next = x + step;
            double sign_next = signbit(f(x_next));
            
            if (sign_f_x != sign_next)
            {
                // localized a root
                result.push_back(x);
            }

            x = x_next;
            sign_f_x = sign_next;
        }

        return result;
    }
}