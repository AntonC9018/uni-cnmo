#pragma once
#include <stdlib.h>
#include "shared.h"
#include "polynomial.h"

namespace Poly
{
    Polynomial* lagrange_approximate_samples(
        const double xs[], 
        const double ys[], 
        const size_t num_samples)
    {
        Polynomial* result = p_alloc_zeros(num_samples);
        double* t          = array_alloc(num_samples);

        for (size_t i = 0; i < num_samples; i++)
        {
            // reset t to 0
            for (size_t j = 1; j < num_samples; j++)
            {
                t[j] = 0;
            }
            // The result will have to be multiplied by yi, so set it right away.
            t[0] = ys[i];
            size_t current_degree = 1;
            double denominator = 1;

            for (size_t j = 0; j < num_samples; j++)
            {
                if (j != i)
                {
                    // Multiplying by (x - xj) is the same as
                    // Shifting all coefficients right and subtracting the coefficients times xj
                    for (size_t k = current_degree; k >= 1; k--)
                    {
                        t[k] = t[k - 1] - t[k] * xs[j];
                    }
                    t[0] = t[0] * -xs[j];

                    // The denominator gets multiplied by the difference
                    denominator *= (xs[i] - xs[j]);

                    // One more degree is non-zero now
                    current_degree++;
                }
            }

            // Invert the denominator
            double r_denominator = 1 / denominator;

            for (size_t j = 0; j < num_samples; j++)
            {
                // Multiply it into all of the coefficients, when adding
                result->coefficients[j] += r_denominator * t[j];
            }
        }

        free(t);

        return result;
    }

    template<typename Function>
    Polynomial* lagrange_approximate_function(
        Function f, 
        const size_t degree,
        const double start = 0.0, 
        const double step = 1.0)
    {
        double x = start;
        double* xs = array_alloc(degree);
        double* ys = array_alloc(degree);

        for (size_t i = 0; i < degree; i++)
        {
            xs[i] = x;
            ys[i] = f(x);
            x += step;
        }

        auto result = lagrange_approximate_samples(xs, ys, degree);

        free(xs); free(ys);

        return result;
    }

    template<typename Function>
    Polynomial* lagrange_approximate_function_chebyshev_nodes(
        Function f, 
        const size_t degree,
        const double start = -1.0, 
        const double end = 1.0)
    {
        double* xs = chebyshev_nodes(f, degree, start, end);
        double* ys = &xs[degree];

        auto result = lagrange_approximate_samples(xs, ys, degree);

        free(xs);

        return result;
    }

}