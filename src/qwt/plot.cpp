#include "plot.h"
#include <algorithm>

#include "../builtin.h"


Plot::Plot(QWidget *parent)
    : QwtPlot(parent)
    , canvas(this)
    , tracker(&canvas)
    , zero_scale(QwtScaleDraw::BottomScale, 0.0)
{
    // we want to have the axis scales like a frame around the
    // canvas
    plotLayout()->setAlignCanvasToScales(true);
    for (int axis = 0; axis < QwtPlot::axisCnt; axis++)
        axisWidget(axis)->setMargin(0);
    {
        QPalette palette;
        palette.setColor(QPalette::Foreground, QColor("Blue"));
        palette.setColor(QPalette::Text,       QColor("Blue"));
        zero_scale.setPalette(palette);
        zero_scale.attach(this);
    }
    {
        canvas.setAutoFillBackground(false);
        canvas.setFrameStyle(QFrame::NoFrame);
        setCanvas(&canvas);
    }
    {
        QwtText title("Graph");
        title.setColor(Qt::black);
        title.setRenderFlags( Qt::AlignHCenter | Qt::AlignTop );

        QFont font;
        font.setBold(true);
        title.setFont(font);

        label.setText(title);
        label.attach(this);
    }
    {
        grid.setMajorPen( Qt::gray, 0, Qt::SolidLine );
        grid.setMinorPen( Qt::gray,  0, Qt::SolidLine );
        grid.attach(this);
    }
    {
        curve.setPen("DarkOrange", 2);
        curve.setRenderHint(QwtPlotItem::RenderAntialiased, true);
        curve.attach(this);
    }
    {
        // for the demo we want the tracker to be active without
        tracker.setStateMachine(new QwtPickerTrackerMachine());
        tracker.setRubberBandPen(QPen("MediumOrchid"));
    }

    updateCurve(&Builtin::funcs[0]);
    updateCurve(&Builtin::funcs[1]);
}

void Plot::updateCurve(const Func* func)
{
    const size_t num_points = 1000;
    const double LEEWAY_Y_PERC = 0.1;
    const double LEEWAY_X_PERC = 0.1;

    auto xys = func_eval_range(func, num_points);
    auto data = new QwtPointArrayData(xys.inputs, xys.outputs);

    curve.setTitle(func->text.chars);
    curve.setData(data);

    double min_y = *std::min_element(xys.outputs.constBegin(), xys.outputs.constEnd());
    double max_y = *std::max_element(xys.outputs.constBegin(), xys.outputs.constEnd());

    double leeway_y = (max_y - min_y) * LEEWAY_Y_PERC;
    double leeway_x = (func->upper_bound - func->lower_bound) * LEEWAY_X_PERC;

    setAxisScale(QwtPlot::yLeft, min_y - leeway_y, max_y + leeway_y);
    setAxisScale(QwtPlot::xBottom, func->lower_bound - leeway_x, func->upper_bound + leeway_x);
}