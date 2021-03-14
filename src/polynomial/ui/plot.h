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

            Inputs_And_Outputs xys;
            xys.inputs.reserve(num_evals);
            xys.outputs.reserve(num_evals);

            for (size_t i = 0; i < num_evals; i++)
            {
                xys.inputs.push_back(x);
                xys.outputs.push_back(func(x));
                x += step;
            }

            auto data = new QwtPointArrayData(xys.inputs, xys.outputs);
            _poly_curve.setData(data);
            replot();
        }

        void update_markers(double* xs, double* ys, size_t count);

    private:
        QwtPlotCurve _poly_curve;
        std::vector<QwtPlotMarker*> _markers;
    };
}