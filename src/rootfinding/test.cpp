#include "stdio.h"

#include "./bisection.h"
#include "./chord.h"
#include "./newton.h"
#include "./secant.h"

#define Vector_Type std::vector<double>
#include "../func/builtin.h"
#include "option.h"

void test();
void profile();

int main()
{
    // test();
    profile();

    return 0;
}

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

void test()
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
}

void profile()
{
    for (auto& func : Builtin::funcs)
    {
        using namespace Root_Finding;
        
        const double num_steps = 25.0;
        const double step = (func.upper_bound - func.lower_bound) / num_steps;
        const auto zeros_xs = localize(func, func.lower_bound, func.upper_bound, step);

        for (size_t option_index = 0; option_index < countof(option_adaptors); option_index++)
        {
            auto option_adaptor = option_adaptors[option_index];
            auto name = option_text[option_index];
            
            Error_Data error_data;
            error_data.error_message = STR_NULL;
            error_data.max_iters = 1000;
            error_data.tolerance = pow(10, -10);
            
            #define _x(p) zeros_xs[(p)->num_experiments % zeros_xs.size()]
            #define inter(p) { _x((p)), _x((p)) + step }

            const size_t num_experiments = 50000;

            char buffer[1024];
            sprintf(buffer, "%s method for function %s", name.chars, func.text.chars);

            profiler_perform_experiments(
                buffer,
                [&](auto p) { option_adaptor(&func, inter(p), &error_data, p); },
                num_experiments
            );
        }
    }
}