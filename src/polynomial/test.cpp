#include "polynomial.h"

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
}