#include "util.h"

namespace Root_Finding
{
    template<typename Function>
    inline bool chord_conditions_met(Function f, double start, double end)
    {
        return signbit(f(start)) != signbit(f(end)); 
    }

    template<typename Function>
    double chord(Function f, double start, double end, 
        double tolerance, size_t max_iters, bool* error)
    {
        RF_ERROR_IF_FALSE(chord_conditions_met(f, start, end));
        RF_ERROR();
    }
}