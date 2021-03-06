#pragma once
#include <vector>
#include <stdlib.h>
#include <math.h>

namespace Root_Finding
{
#define RF_ERROR_IF_FALSE(expr) { *error = !(expr); if (*error) return 0; }
#define RF_ERROR() { *error = true; return 0; }

    inline double abs(double a) { return signbit(a) ? -a : a; }

    // Numerically differentiate function f at point x with step delta
    template<typename Function>
    inline double differentiate(Function f, double x, double delta)
    {
        return (f(x + delta) - f(x)) / delta;
    }

    template<typename Function>
    std::vector<double> localize(Function f, double start, double end, double step)
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