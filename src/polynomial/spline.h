#pragma once
#include "shared.h"
#include "polynomial.h"

namespace Poly
{
    struct Cubic_Spline
    {
        u32 num_samples;
        double data[0];

        inline double operator()(double x) const;
    };

    inline Cubic_Spline* spline_alloc(const size_t num_samples)
    {
        size_t size_xs = sizeof(double) * num_samples;
        size_t size_coeffs = sizeof(double) * (num_samples - 1) * 4;
        size_t size = size_xs + size_coeffs + sizeof(Cubic_Spline);

        auto spline = (Cubic_Spline*) malloc(size);
        spline->num_samples = num_samples;

        return spline;
    }

    inline double* spline_xs(Cubic_Spline* spline)
    {
        return spline->data;
    }

    inline double* spline_coeffs(Cubic_Spline* spline)
    {
        return spline->data + sizeof(double) * spline->num_samples;
    }

    inline double* spline_coeffs_ith(Cubic_Spline* spline, const u32 i)
    {
        return &(spline_coeffs(spline)[4 * i]);
    }

    inline const double* spline_xs(const Cubic_Spline* spline)
    {
        return spline->data;
    }

    inline const double* spline_coeffs(const Cubic_Spline* spline)
    {
        return spline->data + sizeof(double) * spline->num_samples;
    }

    inline const double* spline_coeffs_ith(const Cubic_Spline* spline, const u32 i)
    {
        return &(spline_coeffs(spline)[4 * i]);
    }

    double spline_eval_ith(const Cubic_Spline* spline, const u32 i, const double x)
    {
        const double* coeffs = spline_coeffs_ith(spline, i);
        const double xi = spline_xs(spline)[i];
        double result  = 0;
        double x_power = 1;
        for (u32 i = 0; i < 4; i++)
        {
            result += coeffs[i] * x_power;
            x_power *= (x - xi);
        }
        return result;
    }

    double spline_eval(const Cubic_Spline* spline, const double x)
    {
        const double* xs = spline_xs(spline);
        if (xs[0] > x) 
        {
            return 0;
        }
        for (u32 i = 1; i < spline->num_samples; i++)
        {
            if (xs[i] >= x)
            {
                return spline_eval_ith(spline, i - 1, x);
            }
        }
        return 0;
        /*
        // binary search
        size_t start = 0;
        size_t end = spline->num_samples - 2;

        while (end > start)
        {
            size_t middle = (end + start) / 2;
            if (xs[middle] >= x && xs[middle + 1] < x)
            {
                return spline_eval_ith(spline, middle, x);
            }
            else if (xs[middle] < x)
            {
                start = middle + 1;
            }
            else
            {
                end = middle - 1;
            }
        }

        return spline_eval_ith(spline, start, x);
        */
        
    }

    // It is not very optimized, but I'm tired of this writing out formulas stuff.
    // It works, though.
    Cubic_Spline* make_cubic_spline_normal(
        const double xs[], 
        const double ys[], 
        const u32 num_samples)
    {
        u32 num_polynomials = num_samples - 1;
        
        double* const temp_values = array_alloc(num_samples * 4 + num_polynomials * 4);
        double* const l = temp_values;             // some temporary
        double* const u = &l[num_samples];         // some temporary
        double* const z = &u[num_samples];         // some temporary
        double* const c = &z[num_samples];         // 3rd coeff
        double* const h = &c[num_samples];         // some temporary
        double* const d = &h[num_polynomials];     // 4th coeff
        double* const b = &d[num_polynomials];     // 2nd coeff
        double* const alpha = &b[num_polynomials]; // some temporary

        for (u32 i = 0; i < num_polynomials; i++)
        {
            h[i] = xs[i + 1] - xs[i];
        }

        alpha[0] = 0;
        for (u32 i = 1; i < num_polynomials; i++)
        {
            alpha[i] = 3.0 / h[i] * (ys[i + 1] - ys[i]) - 3.0 / h[i - 1] * (ys[i] - ys[i - 1]);
        }

        l[0] = 1;
        u[0] = 0;
        z[0] = 0;
        l[num_samples - 1] = 1;
        z[num_samples - 1] = 0;
        c[num_samples - 1] = 0;

        for (u32 i = 1; i < num_polynomials; i++)
        {
            l[i] = 2 * (xs[i + 1] - xs[i - 1]) - h[i - 1] * u[i - 1];
            u[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i]; 
        }

        for (s32 i = num_polynomials - 1; i >= 0; i--)
        {
            c[i] = z[i] - u[i] * c[i + 1];
            b[i] = (ys[i + 1] - ys[i]) / h[i] - h[i] * (c[i + 1] + 2 * c[i]) / 3;
            d[i] = (c[i + 1] - c[i]) / (3 * h[i]);
        }

        auto spline = spline_alloc(num_samples);
        memcpy(spline_xs(spline), xs, sizeof(double) * num_samples);

        for (u32 i = 0; i < num_polynomials; i++)
        {
            double* coeffs = spline_coeffs_ith(spline, i);
            coeffs[0] = ys[i];
            coeffs[1] = b[i];
            coeffs[2] = c[i];
            coeffs[3] = d[i];
        }

        free(temp_values);
        return spline;
    }

    inline double Cubic_Spline::operator()(double x) const
    { 
        return spline_eval(this, x); 
    }
} 
