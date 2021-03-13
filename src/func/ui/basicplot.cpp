#include "basicplot.h"
#include <algorithm>



BasicPlot::BasicPlot(QWidget *parent)
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

void BasicPlot::update_curve(Expression_Func* func)
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

    setAxisScale(QwtPlot::yLeft, min_y - leeway_y, max_y + leeway_y);
    setAxisScale(QwtPlot::xBottom, func->lower_bound - leeway_x, func->upper_bound + leeway_x);
    replot();
}