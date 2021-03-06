#include "stdio.h"

#include "./bisection.h"
#include "./chord.h"
#include "./newton.h"
#include "./secant.h"


double func(double x)
{
    return x * x - 1;
}

double func_derivative(double x)
{
    return 2 * x;
}

double func_second_derivative(double x)
{
    return 2;
}

int main()
{
    using namespace Root_Finding;

    {
        const double step = 0.05;

        auto result = localize(func, -9, 9, step);

        for (auto& x : result)
        {
            printf("Got an interval from %f to %f.\n", x, x + step);
        }
    }

    bool error; double result;
    #define PRINT_RESULT(method_name) {\
        printf((method_name)); \
        if (error)\
            printf("An error has occured\n");\
        else\
            printf("Result is: %f\n", result);\
    }\

    const double x0 = 0.5;
    const double x1 = 1.5;
    const double tolerance = 0.00001;
    const double max_iters = 1000;

    result = bisection(func, x0, x1, tolerance, max_iters, &error);
    PRINT_RESULT("Bisection. ");
    result = secant(func, x0, x1, tolerance, max_iters, &error);
    PRINT_RESULT("Secant. ");
    result = newton(func, func_derivative, x0, tolerance, max_iters, &error);
    PRINT_RESULT("Newton. ");
    result = chord(func, func_second_derivative, x0, x1, tolerance, max_iters, &error);
    PRINT_RESULT("Chord. ");
    result = secant_enhanced_start(func, func_second_derivative, x0, x1, tolerance, max_iters, &error);
    PRINT_RESULT("Secant v2. ");

    return 0;
}