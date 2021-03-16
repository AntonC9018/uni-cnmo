#pragma once
#include <stdlib.h>
#include "shared.h"
#include "polynomial.h"

namespace Poly
{
    Polynomial* lagrange_approximate_samples(
        const double xs[], 
        const double ys[], 
        size_t num_samples)
    {
        Polynomial* result = p_alloc_zeros(num_samples);
        double* t = array_alloc(num_samples);

        for (size_t i = 0; i < num_samples; i++)
        {
            // reset t to 0
            memset(&t[1], 0, (num_samples - 1) * sizeof(double));

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
        Function& f, 
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
        Function& f, 
        const size_t degree,
        const double start = -1.0, 
        const double end = 1.0)
    {
        double* xs = chebyshev_nodes(f, degree, start, end);
        auto result = lagrange_approximate_samples(SAMPLES_CONTIGUOUS(xs, degree), degree);

        free(xs);

        return result;
    }

    #define LAGRANGE_SAMPLES(xs, samp, port) (xs), &((xs)[(samp) * (port)]), (samp), (port)
    #define LAGRANGE_TO_NORMAL_SAMPLES(xs, samp, port) (xs), &((xs)[(samp) * (port)]), (samp) * (port)

    template<typename Algo, typename Function>
    double* lagrange_sample_portions(
        Algo& algo, 
        Function& f, 
        size_t num_samples, 
        int num_portions, 
        double start, 
        double end)
    {
        const double outer_step = (end - start) / num_portions;
        double* xs = array_alloc(num_samples * num_portions * 2);
        double* ys = &xs[num_portions * num_samples];
        double x = start;

        for (int i = 0; i < num_portions; i++)
        {
            double x1 = x + outer_step;
            algo(f, &xs[i * num_samples], &ys[i * num_samples], num_samples, x, x1);
            x = x1;
        } 

        return xs;
    }

    // This is a lazy way of doing what I did with splines,
    // but for a variable number of coefficients for the polynomials.
    // Now, it is not required in my case, I'm just being lazy.
    struct Polynomial_Portions
    {
        size_t num_portions;
        double* xs;
        Polynomial** polynomials;

        inline double operator()(double x)
        {
            for (size_t i = 0; i < num_portions - 1; i++)
            {
                if (x < xs[i])
                {
                    return p_eval(polynomials[i], x);
                }
            }
            return p_eval(polynomials[num_portions - 1], x);
        }
    };

    Polynomial_Portions p_make_portions(
        const double* xs, 
        size_t num_samples, 
        size_t num_portions)
    {
        Polynomial_Portions result;
        result.xs = array_alloc(num_portions - 1);
        result.polynomials  = (Polynomial**) malloc(sizeof(Polynomial*) * num_portions);
        result.num_portions = num_portions;
        
        for (size_t i = 0; i < num_portions - 1; i++)
        {
            const double* _xs = &xs[i * num_samples];
            result.xs[i] = (_xs[num_samples - 1] + _xs[num_samples]) / 2;
        }

        return result;
    }

    Polynomial_Portions lagrange_approximate_samples_portions(
        const double* xs,
        const double* ys,
        size_t num_samples, 
        size_t num_portions)
    {
        auto result = p_make_portions(xs, num_samples, num_portions);
        
        for (size_t i = 0; i < num_portions; i++)
        {
            result.polynomials[i] = lagrange_approximate_samples(
                &xs[i * num_samples], &ys[i * num_samples], num_samples
            );
        }

        return result;
    }

    Polynomial_Portions lagrange_node_polynomial_portions(
        const double* xs, 
        size_t num_samples, 
        size_t num_portions)
    {
        auto result = p_make_portions(xs, num_samples, num_portions);
        
        for (size_t i = 0; i < num_portions; i++)
        {
            result.polynomials[i] = node_polynomial(
                &xs[i * num_samples], num_samples
            );
        }

        return result;
    }

    void lagrange_free_portions(Polynomial_Portions portions)
    {
        free(portions.xs);
        for (size_t i = 0; i < portions.num_portions; i++)
        {
            free(portions.polynomials[i]);
        }
        free(portions.polynomials);
    }
}