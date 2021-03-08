#include "plot.h"
#include <algorithm>

#include "../builtin.h"
#include "../rootfinding/shared.h"
#include "../rootfinding/bisection.h"
#include "../rootfinding/newton.h"
#include "../rootfinding/secant.h"
#include "../rootfinding/chord.h"

Plot::Plot(QWidget *parent)
    : QwtPlot(parent)
    , _canvas(this)
    , _tracker(&_canvas)
    , _zero_scale(QwtScaleDraw::BottomScale, 0.0)
{
    {
        plotLayout()->setAlignCanvasToScales(true);
        for (int axis = 0; axis < QwtPlot::axisCnt; axis++)
            axisWidget(axis)->setMargin(0);
    }
    {
        QPalette palette;
        palette.setColor(QPalette::Foreground, QColor("Blue"));
        palette.setColor(QPalette::Text,       QColor("Blue"));
        _zero_scale.setPalette(palette);
        _zero_scale.attach(this);
    }
    {
        _canvas.setAutoFillBackground(false);
        _canvas.setFrameStyle(QFrame::NoFrame);
        setCanvas(&_canvas);
    }
    {
        QwtText title("Graph");
        title.setColor(Qt::black);
        title.setRenderFlags( Qt::AlignHCenter | Qt::AlignTop );

        QFont font;
        font.setBold(true);
        title.setFont(font);

        _label.setText(title);
        _label.attach(this);
    }
    {
        _grid.setMajorPen( Qt::gray, 0, Qt::SolidLine );
        _grid.setMinorPen( Qt::gray,  0, Qt::SolidLine );
        _grid.attach(this);
    }
    {
        _curve.setPen("DarkOrange", 2);
        _curve.setRenderHint(QwtPlotItem::RenderAntialiased, true);
        _curve.attach(this);
    }
    {
        // for the demo we want the tracker to be active without
        _tracker.setStateMachine(new QwtPickerTrackerMachine());
        _tracker.setRubberBandPen(QPen("MediumOrchid"));
    }
}

void Plot::update_curve(Func* func)
{
    const size_t num_points = 1000;
    const double LEEWAY_Y_PERC = 0.1;
    const double LEEWAY_X_PERC = 0.1;
    const double MIN_HEIGHT = 0.01;

    auto xys = func_eval_range(func, num_points);
    auto data = new QwtPointArrayData(xys.inputs, xys.outputs);

    _curve.setTitle(func->text.chars);
    _curve.setData(data);

    double min_y = *std::min_element(xys.outputs.constBegin(), xys.outputs.constEnd());
    double max_y = *std::max_element(xys.outputs.constBegin(), xys.outputs.constEnd());

    double leeway_y = (max_y - min_y) * LEEWAY_Y_PERC;
    double leeway_x = (func->upper_bound - func->lower_bound) * LEEWAY_X_PERC;

    double range_y = max_y - min_y;

    if (range_y < MIN_HEIGHT)
    {
        double y = MIN_HEIGHT - range_y;
        max_y += y / 2;
        min_y -= y / 2;
    }

    zeros(func);

    setAxisScale(QwtPlot::yLeft, min_y - leeway_y, max_y + leeway_y);
    setAxisScale(QwtPlot::xBottom, func->lower_bound - leeway_x, func->upper_bound + leeway_x);
    replot();
}

void Plot::zeros(Func* func)
{
    const double num_steps = 200.0;
    double step = (func->upper_bound - func->lower_bound) / num_steps;

    auto zeros_xs_starts = Root_Finding::localize<Func&>(
        *func, func->lower_bound, func->upper_bound, step);

    reset_number_of_zero_markers(zeros_xs_starts.size());

    for (size_t i = 0; i < zeros_xs_starts.size(); i++)
    {
        Root_Finding::Error_Data error_data;
        error_data.error_message = STR_NULL;
        error_data.max_iters = 1000;
        error_data.tolerance = 0.0001;

        auto estimate = Root_Finding::bisection<Func&>(
            *func, 
            { zeros_xs_starts[i], zeros_xs_starts[i] + step },
            &error_data
        );

        if (!str_is_null(error_data.error_message))
        {
            puts(error_data.error_message.chars);
        }
        else
        {
            printf("Found a zero at %f.", estimate);
            _zeros[i]->setValue(QPointF(estimate, (*func)(estimate)));
        }
    }
}

void Plot::reset_number_of_zero_markers(size_t new_number)
{
    size_t current_number = _zeros.size();
    // Add more markers, if the current amount does not suffice.
    for (size_t i = current_number; i < new_number; i++)
    {
        auto* marker = new QwtPlotMarker();
        _zeros.push_back(marker);
        // This functions does take ownership of the symbol
        marker->setSymbol(new QwtSymbol(QwtSymbol::Diamond, Qt::red, Qt::NoPen, QSize(5, 5)));
        marker->attach(this);
    }
    // Show all active markers
    for (size_t i = 0; i < new_number; i++)
    {
        _zeros[i]->show();
    }
    // Hide marker excess.
    for (size_t i = new_number; i < current_number; i++)
    {
        _zeros[i]->hide();
    }
}