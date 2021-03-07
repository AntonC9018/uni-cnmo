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

    Error_Data error_data;
    error_data.error_message = STR_NULL;
    error_data.max_iters = 1000;
    error_data.tolerance = 0.00001;

    double result;
    #define PRINT_RESULT(method_name) {\
        printf((method_name)); \
        if (!str_is_null(error_data.error_message))\
            printf("An error has occured: %s\n", error_data.error_message.chars);\
        else\
            printf("Result is: %f\n", result);\
    }\

    const Interval inter = { 0.5, 1.5 };

    result = bisection(func, inter, &error_data);
    PRINT_RESULT("Bisection. ");
    result = secant(func, inter.start, inter.end, &error_data);
    PRINT_RESULT("Secant. ");
    result = newton(func, func_derivative, inter.start, &error_data);
    PRINT_RESULT("Newton. ");
    result = chord(func, func_second_derivative, inter, &error_data);
    PRINT_RESULT("Chord. ");
    result = secant_enhanced_start(func, func_second_derivative, inter, &error_data);
    PRINT_RESULT("Secant v2. ");
    result = newton_enhanced_start(func, func_derivative, func_second_derivative, inter, &error_data);
    PRINT_RESULT("Newton v2. ");

    return 0;
}