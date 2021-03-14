#pragma once
#include <mydefines.h>
#include <string.h>
#include <math.h>

namespace Poly
{
    inline double* array_alloc(size_t length)
    {
        size_t size = length * sizeof(double);
        return (double*)malloc(size);
    }

    inline double* array_of(size_t length, double value)
    {
        double* result = array_alloc(length);
        for (size_t i = 0; i < length; i++)
        {
            result[i] = value;
        }
        return result;
    }

    inline double array_sum(double* start, size_t count)
    {
        double result = 0;
        for (size_t i = 0; i < count; i++)
        {
            result += start[count];
        }
        return result;
    }

    // Returns an array of x's of size degree and an array of y's, right after it in memory.
    // The array contains the rescaled roots of chebyshev polynomial as x's
    // and the values of the function at those x's as y's.
    template<typename Function>
    double* chebyshev_nodes(
        Function& f, 
        const size_t degree,
        const double start = -1.0, 
        const double end = 1.0)
    {
        double* xs = (double*)malloc(degree * 2 * sizeof(double));
        double* ys = &xs[degree];

        const double middle = (end + start) / 2;
        const double half_length = (end - start) / 2;

        for (size_t i = 0; i < degree; i++)
        {
            // Calculate the ith root of chebyshev polynomial.
            // These values as sample x's minimize error on the given interval.
            const double chebyshev_node = cos(M_PI * ((double)i + 0.5) / (double)degree);

            // Rescale the root, since it is bound between [-1, 1]
            const double x = middle + half_length * chebyshev_node;

            xs[i] = x;
            ys[i] = f(x);
        }

        return xs;
    }

    template<typename Function>
    double* random_nodes(
        Function& f, 
        const size_t degree,
        const double start = -1.0, 
        const double end = 1.0)
    {
        double* xs = (double*)malloc(degree * 2 * sizeof(double));
        double* ys = &xs[degree];

        // Generate degree random numbers
        for (size_t i = 0; i < degree; i++)
        {
            xs[i] = rand();
        }
        
        // Normalize the xs to the interval
        const double sum = array_sum(xs, degree);
        const double interval_length = end - start;
        const double scale_factor = interval_length / sum;

        for (size_t i = 0; i < degree; i++)
        {
            xs[i] = xs[i] * scale_factor + start;
            ys[i] = f(xs[i]);
        }

        return xs;
    }

    template<typename Function>
    double* uniform_nodes(
        Function& f, 
        const size_t degree,
        const double start = -1.0, 
        const double end = 1.0)
    {
        double* xs = (double*)malloc(degree * 2 * sizeof(double));
        double* ys = &xs[degree];

        xs[0] = start; ys[0] = f(start);

        if (degree > 1)
        {
            double const step = (end - start) / ((double)degree - 1);

            for (size_t i = 1; i < degree; i++)
            {
                xs[i] = xs[i - 1] + step;
                ys[i] = f(xs[i]);
            }
        }

        return xs;
    }

    #define SAMPLES_CONTIGUOUS(xs, degree) (xs), &(xs)[degree]
    #define SAMPLES(xs, degree) SAMPLES_CONTIGUOUS((xs), (degree)), (degree) 
}