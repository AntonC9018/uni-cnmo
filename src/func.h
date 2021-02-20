#pragma once
#include <cparse/shunting-yard.h>
#include <strlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define Vector_Type QVector<double>

struct Func
{
    str_view_t text;
    calculator calculator;
    double upper_bound;
    double lower_bound;
};

inline Func func_make(str_view_t text, double lower, double upper)
{
    Func func;
    func.text        = text;
    func.calculator  = text.chars;
    func.lower_bound = lower;
    func.upper_bound = upper;
    return func;
}

// Returns a token map with some basic constants
inline TokenMap make_token_map()
{
    TokenMap token_map;
    token_map["pi"] = M_PI;
    token_map["e"]  = M_E;
    return token_map;
}

inline Vector_Type func_eval(const calculator* calc, const Vector_Type& inputs)
{
    Vector_Type result(inputs.size());
    auto tm = make_token_map(); auto& x = tm["x"];

    for (size_t i = 0; i < result.size(); i++)
    {
        x = inputs[i];
        result.push_back(calc->eval(tm).asDouble());
    }
    return result;
}

struct Inputs_And_Outputs
{
    Vector_Type inputs;
    Vector_Type outputs;
};

inline Inputs_And_Outputs func_eval_range(const Func* func, size_t num_evals)
{
    double step = (func->upper_bound - func->lower_bound) / num_evals;
    auto tm = make_token_map(); auto& x = tm["x"];

    Inputs_And_Outputs result;
    result.inputs.reserve(num_evals);
    result.outputs.reserve(num_evals);

    double current_input = func->lower_bound;

    for (size_t i = 0; i < num_evals; i++)
    {
        x = current_input;
        result.inputs .push_back(current_input);
        result.outputs.push_back(func->calculator.eval(tm).asDouble());
        current_input += step;
    }

    return result;
}