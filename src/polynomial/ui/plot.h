#pragma once
#include <func/ui/basicplot.h>
#include <func/func.h>

namespace Poly
{
    class Plot: public BasicPlot
    {
        Q_OBJECT

    public:
        explicit Plot(QWidget* parent = NULL);

        template<typename Function>
        void update_poly_curve(Function& func, double start, double end)
        {
            const double num_evals = 1000;
            const double step = (end - start) / (double)num_evals;
            double x = start;

            QVector<double> vec_x(num_evals);
            QVector<double> vec_y(num_evals);

            for (size_t i = 0; i < num_evals; i++)
            {
                vec_x[i] = x;
                vec_y[i] = func(x);
                x += step;
            }

            auto data = new QwtPointArrayData(vec_x, vec_y);
            _poly_curve.setData(data);
            replot();
        }

        void update_markers(double* xs, double* ys, size_t count);

    private:
        QwtPlotCurve _poly_curve;
        std::vector<QwtPlotMarker*> _markers;
    };
}