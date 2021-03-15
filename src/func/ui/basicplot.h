#pragma once

#include <qwt_plot.h>
#include <qwt_point_data.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_textlabel.h>
#include <qwt_plot_zoneitem.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_scale_widget.h>
#include <qwt_symbol.h>
#include <qwt_plot_scaleitem.h>

#include "curvetracker.h"
#include "../func.h"
#include <algorithm>

class BasicPlot: public QwtPlot
{
    Q_OBJECT

public:
    explicit BasicPlot(QWidget * parent = NULL);
    void update_curve(Expression_Func* func);

    template<typename Function>
    static Inputs_And_Outputs func_eval_range(
        Function& func, double start, double end, size_t num_evals = 1000)
    {
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

        return xys;
    }
    
    template<typename Function>
    void update_curve(Function& f, double start, double end)
    {
        const size_t num_points = 1000;
        const double LEEWAY_Y_PERC = 0.1;
        const double LEEWAY_X_PERC = 0.1;
        const double MIN_HEIGHT = 0.01;

        auto xys = func_eval_range(f, start, end, num_points);
        auto data = new QwtPointArrayData(xys.inputs, xys.outputs);

        _curve.setData(data);

        double min_y = *std::min_element(xys.outputs.constBegin(), xys.outputs.constEnd());
        double max_y = *std::max_element(xys.outputs.constBegin(), xys.outputs.constEnd());

        double leeway_y = (max_y - min_y) * LEEWAY_Y_PERC;
        double leeway_x = (end - start) * LEEWAY_X_PERC;

        double range_y = max_y - min_y;

        if (range_y < MIN_HEIGHT)
        {
            double y = MIN_HEIGHT - range_y;
            max_y += y / 2;
            min_y -= y / 2;
        }

        setAxisScale(QwtPlot::yLeft, min_y - leeway_y, max_y + leeway_y);
        setAxisScale(QwtPlot::xBottom, start - leeway_x, end + leeway_x);
        replot();
    }

    void set_label(str_view_t name);

private:
    QwtPlotScaleItem _zero_scale;
    QwtPlotCanvas _canvas;
    QwtPlotTextLabel _label;
    QwtPlotGrid _grid;
    QwtPlotCurve _curve;
    CurveTracker _tracker;
};