#pragma once
#include <assert.h>
#include "shared.h"

namespace Poly
{
    struct Polynomial
    {
        u32 degree;
        double coefficients[1]; // flexible array

        inline double operator()(double x) const;
    };

    inline Polynomial* p_alloc(u32 degree)
    {
        auto p = (Polynomial*) malloc(sizeof(Polynomial) - sizeof(double) + sizeof(double) * degree);
        p->degree = degree;
        return p;
    }

    inline Polynomial* p_alloc_zeros(u32 degree)
    {
        auto p = (Polynomial*) calloc(1, sizeof(Polynomial)- sizeof(double) + sizeof(double) * degree);
        p->degree = degree;
        return p;
    }

    inline Polynomial* p_make(u32 degree, double coeffs[])
    {
        auto p = p_alloc(degree);
        memcpy(p->coefficients, coeffs, degree * sizeof(double));
        return p;
    }

#define POLY_MAKE_STATIC(args) p_make(sizeof((args)) / sizeof(double), (args))

    inline void p_destroy(Polynomial* poly)
    {
        free(poly);
    }

    void p_add(Polynomial* p1, const Polynomial* p2)
    {
        assert(p1->degree >= p2->degree);
        
        for (u32 i = 0; i < p2->degree; i++)
        {
            p1->coefficients[i] += p2->coefficients[i];
        }
    }

    double p_eval(const Polynomial* p, double x)
    {
        double result = 0;
        double x_power = 1;

        for (u32 i = 0; i < p->degree; i++)
        {
            result += x_power * p->coefficients[i];
            x_power *= x;
        }

        return result;
    }

    Polynomial* p_differentiate(const Polynomial* p)
    {
        if (p->degree == 0) return p_alloc(0);

        auto p_new = p_alloc(p->degree - 1);

        for (u32 i = 1; i < p->degree; i++)
        {
            p_new->coefficients[i - 1] = p->coefficients[i] * (double)i;
        }

        return p_new;
    }

    Polynomial* p_integrate(const Polynomial* p)
    {
        auto p_new = p_alloc(p->degree + 1);
        p_new->coefficients[0] = 0;

        for (u32 i = 0; i < p->degree; i++)
        {
            p_new->coefficients[i + 1] = p->coefficients[i] / (double)(i + 1);
        }

        return p_new;
    }

    void p_print(const Polynomial* p, FILE* file = stdout)
    {
        if (p->degree == 0)
        {
            fputc('0', file);
        }
        else
        {
            fprintf(file, "%.2f", p->coefficients[0]);

            if (p->degree >= 2)
            {
                fprintf(file, " + %.2f * x", p->coefficients[1]); 

                for (u32 i = 2; i < p->degree; i++)
                {
                    fprintf(file, " + %.2f * x^%u", p->coefficients[i], i); 
                }
            }
        }
    }

    inline double Polynomial::operator()(double x) const
    { 
        return p_eval(this, x); 
    }

    Polynomial* node_polynomial(const double* xs, const u32 num_samples)
    {
        auto result = p_alloc_zeros(num_samples + 1);
        auto t = &result->coefficients[0];
        t[0] = 1;
        u32 current_degree = 1;

        for (u32 i = 0; i < num_samples; i++)
        {
            for (u32 k = current_degree; k >= 1; k--)
            {
                t[k] = t[k - 1] - t[k] * xs[i];
            }
            t[0] = t[0] * -xs[i];
            current_degree++;
        }

        return result;
    }
}