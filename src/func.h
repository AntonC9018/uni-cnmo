#pragma once
#include <tinyexpr.h>
#include <strlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define Vector_Type QVector<double>

struct Expression_Func
{
    str_view_t text;
    te_expr* expr;
    te_variable variable;
    double x;
    double upper_bound;
    double lower_bound;

    inline double operator()(double x);
};

inline double Expression_Func::operator()(double x)
{
    this->x = x;
    return te_eval(this->expr);
}

inline void func_compile(Expression_Func* func, int* error = NULL)
{
    func->expr = te_compile(func->text.chars, &func->variable, 1, error);
}

inline Expression_Func func_make(str_view_t text, double lower, double upper, int* error = NULL)
{
    Expression_Func func;
    func.text        = text;
    func.variable    = { "x", &func.x };
    func.lower_bound = lower;
    func.upper_bound = upper;
    func_compile(&func, error);
    return func;
}

inline Vector_Type func_eval(Expression_Func* func, const Vector_Type& inputs)
{
    Vector_Type result(inputs.size());

    for (size_t i = 0; i < result.size(); i++)
    {
        func->x = inputs[i];
        result.push_back(te_eval(func->expr));
    }
    return result;
}

struct Inputs_And_Outputs
{
    Vector_Type inputs;
    Vector_Type outputs;
};

inline Inputs_And_Outputs func_eval_range(Expression_Func* func, size_t num_evals)
{
    double step = (func->upper_bound - func->lower_bound) / num_evals;

    Inputs_And_Outputs result;
    result.inputs.reserve(num_evals);
    result.outputs.reserve(num_evals);

    func->x = func->lower_bound;

    for (size_t i = 0; i < num_evals; i++)
    {
        result.inputs .push_back(func->x);
        result.outputs.push_back(te_eval(func->expr));
        func->x += step;
    }

    return result;
}

struct Derivative_Expression_Func
{
    Expression_Func* initial_func;
    te_expr* expr;
    inline double operator()(double x);
};

inline double Derivative_Expression_Func::operator()(double x)
{
    this->initial_func->x = x;
    return te_eval(this->expr);
}