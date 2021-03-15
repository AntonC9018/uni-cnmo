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
            auto xys = func_eval_range(func, start, end);
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