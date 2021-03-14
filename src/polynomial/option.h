#include "lagrange.h"
#include "spline.h"
#include <func/func.h>

namespace Poly
{
    const str_view_t algo_options[] 
    {
        str_lit("Lagrange"),
        str_lit("Lagrange (portions)"),
        str_lit("Cubic spline")
    };

    const str_view_t sample_options[]
    {
        str_lit("Uniform"),
        str_lit("Random"),
        str_lit("Chebyshev nodes")
    };

    const decltype(&chebyshev_nodes<Expression_Func>) sample_algos[]
    {
        &uniform_nodes<Expression_Func>,
        &random_nodes<Expression_Func>,
        &chebyshev_nodes<Expression_Func>
    };
}