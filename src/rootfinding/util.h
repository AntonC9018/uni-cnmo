#include <stdlib.h>
#include <strlib.h>
#include <math.h>

namespace Root_Finding
{
#define RF_ERROR_IF_FALSE(expr) { *error = (expr); if (*error) return 0; }
#define RF_ERROR() { *error = true; return 0; }

    inline double abs(double a) { return signbit(a) ? -a : a; }

    // Numerically differentiate function f at point x with step delta
    template<typename Function>
    inline double differentiate(Function f, double x, double delta)
    {
        return (f(x + delta) - f(x)) / delta;
    }
}