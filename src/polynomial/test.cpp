#include "polynomial.h"
#include "lagrange.h"
#include "spline.h"

double sin_(double t)
{
    return sin(t);
}

int main()
{
    using namespace Poly;

    Polynomial* p0 = p_alloc(0);

    Polynomial* p1 = p_alloc(1);
    p1->coefficients[0] = -1;

    Polynomial* p2 = p_alloc(2);
    p2->coefficients[0] = -1;
    p2->coefficients[1] = 1;

    Polynomial* p3 = p_alloc(3);
    p3->coefficients[0] = -1;
    p3->coefficients[1] = 1;
    p3->coefficients[2] = 2;

#define EVAL(p, num) \
    p_print((p)); \
    printf(" evaluated at 1 = %f\n", (*(p))((num)));

    EVAL(p0, 2);
    EVAL(p1, 2);
    EVAL(p2, 2);
    EVAL(p3, 2);

    auto p3_diff = p_differentiate(p3);
    p_print(p3); printf(" derivative is "); p_print(p3_diff); printf("\n");

    auto p3_inte = p_integrate(p3);
    p_print(p3); printf(" integral is "); p_print(p3_inte); printf("\n");

    auto p3_approx = lagrange_approximate_function<Polynomial&>(*p3, 3, 1.0, 2.0);
    p_print(p3_approx); printf("\n");


    double coeffs[] { 1, 2, 3, 4, 5 };
    auto p5 = POLY_MAKE_STATIC(coeffs);
    p_print(p5); printf("\n");

    auto p5_approx = lagrange_approximate_function<Polynomial&>(*p5, 5);
    p_print(p5_approx); printf("\n");

    auto p5_approx_chebyshev = lagrange_approximate_function_chebyshev_nodes<Polynomial&>(*p5, 5);
    p_print(p5_approx_chebyshev); printf("\n");

    auto p7_sin = lagrange_approximate_function_chebyshev_nodes(sin_, 7);
    p_print(p7_sin); printf("\n");

    printf("\n\n");
    
    double spline_xs[] { 0, 1, 2, 3, 4, 5 };
    double spline_ys[] { 1, 3, -1, 5, 2, 1 };
    auto spline = make_cubic_spline_normal(spline_xs, spline_ys, 6);
    for (int i = 0; i < 6; i++)
    {
        printf("spline at %i is %f\n", i, spline_eval(spline, spline_xs[i]));
    }
}