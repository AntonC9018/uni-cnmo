#pragma once
#include <strlib.h>
#include <mydefines.h>

#include "../rootfinding/shared.h"
#include "../rootfinding/bisection.h"
#include "../rootfinding/newton.h"
#include "../rootfinding/secant.h"
#include "../rootfinding/chord.h"
#include "../func.h"

namespace Root_Finding
{
    struct Derivative_Expression_Func
    {
        double* x;
        te_expr* expr;
        inline double operator()(double x);
    };

    inline double Derivative_Expression_Func::operator()(double x)
    {
        *this->x = x;
        return te_eval(this->expr);
    }

#define ADAPTOR_PARAMS Expression_Func* f, Interval inter, Error_Data* error_data, Profiler* profiler = &_std_profiler
#define DER(f) ([=](double x) { return func_eval_derivative((f), x); }) 
#define SECOND_DER(f) ([=](double x) { return func_eval_second_derivative((f), x); })

    inline double bisection_adaptor(ADAPTOR_PARAMS)
    {
        return bisection(*f, inter, error_data, profiler);
    }

    inline double chord_adaptor(ADAPTOR_PARAMS)
    {
        return chord(*f, SECOND_DER(f), inter, error_data, profiler);
    }

    inline double newton_adaptor(ADAPTOR_PARAMS)
    {
        return newton_enhanced_start(*f, DER(f), SECOND_DER(f), inter, error_data, profiler);
    }

    inline double secant_adaptor(ADAPTOR_PARAMS)
    {
        return secant_enhanced_start(*f, SECOND_DER(f), inter, error_data, profiler);
    }

    const decltype(&bisection_adaptor) option_adaptors[]
    {
        &bisection_adaptor,
        &chord_adaptor,
        &newton_adaptor,
        &secant_adaptor
    };
    
    const str_view_t option_text[]
    {
        str_lit("Bisection"),
        str_lit("Chord"),
        str_lit("Newton"),
        str_lit("Secant"),
    };

    static_assert(countof(option_adaptors) == countof(option_text));
}