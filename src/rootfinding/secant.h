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
        double tolerance, size_t max_iters, bool* error)
    {
        RF_ERROR_IF_FALSE(secant_conditions_met(f_derivative, x0, x1));

        size_t i = 0;
        double x_prev   = x0;
        double x        = x1;
        double f_x_prev = f(x0);

        while (i < max_iters)
        {
            double f_x = f(x);

            if (abs(f_x) <= tolerance)
                return x;

            double x2 = (x_prev * f_x - x * f_x_prev) / (f_x - f_x_prev);
            x_prev = x; x = x2;
            f_x_prev = f_x; 
            i++;
        }

        RF_ERROR();
    }
}